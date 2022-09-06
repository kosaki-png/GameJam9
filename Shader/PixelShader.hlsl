struct PS_IN
{
        float4 pos : SV_POSITION;
		float3 posw : WORLD_POS;
        float4 nor : NORMAL;
		float4 uv : TEXTURE0;
};

cbuffer ConstantBuffer
{
	float4x4	World;				// ワールド変換行列
	float4x4	View;				// ビュー変換行列
	float4x4	Projection;			// 透視射影変換行列
	float4		CameraPos;			// カメラ座標
	float4		MaterialAmbient;	// アンビエント
	float4		MaterialDiffuse;	// ディフューズ
	float4		MaterialSpecular;	// スペキュラー
	float4		LightPosMain;
	float4		LightPos1;
	float4		LightPos2;
	float		light_amount;
}

Texture2D    Texture : register(t0[0]); // Textureをスロット0の0番目のテクスチャレジスタに設定
SamplerState Sampler : register(s0[0]); // Samplerをスロット0の0番目のサンプラレジスタに設定

static const float PI = 3.141592654f;	// π
static float3 N;	// 反射点の法線ベクトル

// Schlickによる近似
// f0とf90の値を(1 - cosine)の５乗でlerpする
// f0 : 光が垂直に入射したときの反射率
// f90 : 光が平行に入射したときの反射率
// cosine : ２ベクトルのなす角のコサイン
float SchlickFresnel(float f0, float f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}

float3 SchlickFresnel3(float3 f0, float3 f90, float cosine)
{
	float m = saturate(1 - cosine);
	float m2 = m * m;
	float m5 = m2 * m2 * m;
	return lerp(f0, f90, m5);
}

// ディズニーのフレネル計算
float3 DisneyFresnel(float LdotH, float3 baseColor)
{
	// F項
	// 輝度
	float luminance = 0.3f * baseColor.r + 0.6f * baseColor.g + 0.1f * baseColor.b;
	// 色合い
	float3 tintColor = baseColor / luminance;
	// 非金属の鏡面反射色を計算
	float3 nonMetalColor = MaterialSpecular * 0.08f * tintColor;
	// metalnessによる色補間　金属の場合はベースカラー
	float3 specularColor = lerp(nonMetalColor, baseColor, 0.5);
	// NdotHの割合でSchlickFresnel補間
	return SchlickFresnel3(specularColor, float3(1, 1, 1), LdotH);
}

// UE4のGGX分布
// alpha : roughnessの２乗
// NdotH : 法線とハーフベクトルの内積
float DistributionGGx(float alpha, float NdotH)
{
	float alpha2 = alpha * alpha;
	float t = NdotH * NdotH * (alpha2 - 1.0f) + 1.0f;
	return alpha2 / (PI * t * t);
}

// UE4のSmithモデル
float GeometricSmith(float cosine)
{
	float k = (0 + 1.0f);

	k = k * k / 8.0f;

	return cosine / (cosine * (1.0f - k) + k);
}

// 描画反射の計算
float3 CookTorranceSpecular(float NdotL, float NdotV, float NdotH, float LdotH, float3 baseColor)
{
	// D項(分布：Distribution)
	float Ds = DistributionGGx(0.5 * 0.5, NdotH);

	// F項(フレネル：Fresnel)
	float3 Fs = DisneyFresnel(LdotH, baseColor);

	// G項(幾何減衰：Geometry attenuation)
	//float Gs = 1.0f;
	float Gs = GeometricSmith(NdotL) * GeometricSmith(NdotV);

	// m項(分母)
	float m = 4.0f * NdotL * NdotV;

	// 合成して鏡面反射の色を得る
	return Ds * Fs * Gs / m;
}

// 双方向反射分布関数
float3 BRDF(float3 L, float3 V, float3 baseColor)
{
	L = normalize(L);

	// 法線とライトの内積
	float NdotL = dot(N, L);
	// 法線とカメラ方向の内積
	float NdotV = dot(N, V);
	// どちらかが90度以上であれば
	//if (NdotV < 0 || NdotL < -0) { return float3(0, 0, 0); }

	/*if (NdotL < 0) { return (baseColor - baseColor * abs(NdotL)) * 0.05; }*/
	if (NdotV < 0 || NdotL < -0) { return (1.0 - smoothstep(0, -0.2, NdotL) * baseColor) * 0.1; }

	// ライト方向とカメラ方向の中間
	float3 H = normalize(L + V);
	// 法線とハーフベクトルの内積
	float NdotH = dot(N, H);
	// ライトとハーフベクトルの内積
	float LdotH = dot(L, H);

	// 拡散反射率
	float diffuseReflectance = 1.0f / PI;

	// 入射角が90度の場合の拡散反射率
	float energyBias = 0.5f * 0.5;
	float Fd90 = energyBias + 2 * LdotH * LdotH * 0.5;
	// 入っていく時の拡散反射率
	float FL = SchlickFresnel(1.0f, Fd90, NdotL);
	// 出ていく時の拡散反射率
	float FV = SchlickFresnel(1.0f, Fd90, NdotV);
	// 入って出ていくまでの拡散反射率
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, 0.5);
	float Fd = FL * FV * energyFactor;

	// 拡散反射項
	float3 diffuseColor = diffuseReflectance * Fd * baseColor * (1 - 0.5);

	// 鏡面反射項
	float3 specularColor = CookTorranceSpecular(NdotL, NdotV, NdotH, LdotH, baseColor);

	return diffuseColor + specularColor;
}

float4 Diffuse(float4 normal, float4 lightPos, float3 baseColor)
{
	return max(0, dot(normalize(normal.xyz), normalize(lightPos.xyz))) * float4(baseColor, 1);
}

float4 main(PS_IN input) : SV_Target
{
	// 面の情報をstatic変数に代入し、関数から参照できるようにする
	N = input.nor;
	// 最終的なRGB
	float4 finalRGB = float4(0, 0, 0, 0);
	// 頂点から視点への方向ベクトル
	float3 eyedir = normalize(CameraPos - input.posw.xyz);
	float3 lightdirMain = (LightPosMain.xyz - input.posw.xyz);
	float3 lightdirSub1 = (LightPos1.xyz - input.posw.xyz);
	float3 lightdirSub2 = (LightPos2.xyz - input.posw.xyz);
	float4 baseColor = Texture.Sample(Sampler, input.uv);

	float lightLengthMain = length(lightdirMain);
	float lightLengthSub1 = length(lightdirSub1);
	float lightLengthSub2 = length(lightdirSub2);

	// diffuseを計算しBRDFの結果とライト色を合成
	// メインライト
	float4 diffuseColor = Diffuse(input.nor, LightPosMain, baseColor.xyz);
	finalRGB += (float4(BRDF(lightdirMain, eyedir, baseColor.xyz), 1) + diffuseColor) * 0.2;

	// サブライト１
	diffuseColor = Diffuse(input.nor, LightPos1, baseColor.xyz);
	finalRGB += (float4(BRDF(lightdirSub1, eyedir, baseColor.xyz), 1) + diffuseColor) * (1.0 - smoothstep(10.0, 15.0, lightLengthSub1)) * 0.5;

	// サブライト２
	diffuseColor = Diffuse(input.nor, LightPos2, baseColor.xyz);
	finalRGB += (float4(BRDF(lightdirSub2, eyedir, baseColor.xyz), 1) + diffuseColor) * (1.0 - smoothstep(10.0, 15.0, lightLengthSub2)) * 0.5;

	//finalRGB += baseColor * 0.4;
	
	return float4(finalRGB.xyz, 1.0);
}
