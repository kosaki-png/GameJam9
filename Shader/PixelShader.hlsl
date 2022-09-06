struct PS_IN
{
        float4 pos : SV_POSITION;
		float3 posw : WORLD_POS;
        float4 nor : NORMAL;
		float4 uv : TEXTURE0;
};

cbuffer ConstantBuffer
{
	float4x4	World;				// ���[���h�ϊ��s��
	float4x4	View;				// �r���[�ϊ��s��
	float4x4	Projection;			// �����ˉe�ϊ��s��
	float4		CameraPos;			// �J�������W
	float4		MaterialAmbient;	// �A���r�G���g
	float4		MaterialDiffuse;	// �f�B�t���[�Y
	float4		MaterialSpecular;	// �X�y�L�����[
	float4		LightPosMain;
	float4		LightPos1;
	float4		LightPos2;
	float		light_amount;
}

Texture2D    Texture : register(t0[0]); // Texture���X���b�g0��0�Ԗڂ̃e�N�X�`�����W�X�^�ɐݒ�
SamplerState Sampler : register(s0[0]); // Sampler���X���b�g0��0�Ԗڂ̃T���v�����W�X�^�ɐݒ�

static const float PI = 3.141592654f;	// ��
static float3 N;	// ���˓_�̖@���x�N�g��

// Schlick�ɂ��ߎ�
// f0��f90�̒l��(1 - cosine)�̂T���lerp����
// f0 : ���������ɓ��˂����Ƃ��̔��˗�
// f90 : �������s�ɓ��˂����Ƃ��̔��˗�
// cosine : �Q�x�N�g���̂Ȃ��p�̃R�T�C��
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

// �f�B�Y�j�[�̃t���l���v�Z
float3 DisneyFresnel(float LdotH, float3 baseColor)
{
	// F��
	// �P�x
	float luminance = 0.3f * baseColor.r + 0.6f * baseColor.g + 0.1f * baseColor.b;
	// �F����
	float3 tintColor = baseColor / luminance;
	// ������̋��ʔ��ːF���v�Z
	float3 nonMetalColor = MaterialSpecular * 0.08f * tintColor;
	// metalness�ɂ��F��ԁ@�����̏ꍇ�̓x�[�X�J���[
	float3 specularColor = lerp(nonMetalColor, baseColor, 0.5);
	// NdotH�̊�����SchlickFresnel���
	return SchlickFresnel3(specularColor, float3(1, 1, 1), LdotH);
}

// UE4��GGX���z
// alpha : roughness�̂Q��
// NdotH : �@���ƃn�[�t�x�N�g���̓���
float DistributionGGx(float alpha, float NdotH)
{
	float alpha2 = alpha * alpha;
	float t = NdotH * NdotH * (alpha2 - 1.0f) + 1.0f;
	return alpha2 / (PI * t * t);
}

// UE4��Smith���f��
float GeometricSmith(float cosine)
{
	float k = (0 + 1.0f);

	k = k * k / 8.0f;

	return cosine / (cosine * (1.0f - k) + k);
}

// �`�攽�˂̌v�Z
float3 CookTorranceSpecular(float NdotL, float NdotV, float NdotH, float LdotH, float3 baseColor)
{
	// D��(���z�FDistribution)
	float Ds = DistributionGGx(0.5 * 0.5, NdotH);

	// F��(�t���l���FFresnel)
	float3 Fs = DisneyFresnel(LdotH, baseColor);

	// G��(�􉽌����FGeometry attenuation)
	//float Gs = 1.0f;
	float Gs = GeometricSmith(NdotL) * GeometricSmith(NdotV);

	// m��(����)
	float m = 4.0f * NdotL * NdotV;

	// �������ċ��ʔ��˂̐F�𓾂�
	return Ds * Fs * Gs / m;
}

// �o�������˕��z�֐�
float3 BRDF(float3 L, float3 V, float3 baseColor)
{
	L = normalize(L);

	// �@���ƃ��C�g�̓���
	float NdotL = dot(N, L);
	// �@���ƃJ���������̓���
	float NdotV = dot(N, V);
	// �ǂ��炩��90�x�ȏ�ł����
	//if (NdotV < 0 || NdotL < -0) { return float3(0, 0, 0); }

	/*if (NdotL < 0) { return (baseColor - baseColor * abs(NdotL)) * 0.05; }*/
	if (NdotV < 0 || NdotL < -0) { return (1.0 - smoothstep(0, -0.2, NdotL) * baseColor) * 0.1; }

	// ���C�g�����ƃJ���������̒���
	float3 H = normalize(L + V);
	// �@���ƃn�[�t�x�N�g���̓���
	float NdotH = dot(N, H);
	// ���C�g�ƃn�[�t�x�N�g���̓���
	float LdotH = dot(L, H);

	// �g�U���˗�
	float diffuseReflectance = 1.0f / PI;

	// ���ˊp��90�x�̏ꍇ�̊g�U���˗�
	float energyBias = 0.5f * 0.5;
	float Fd90 = energyBias + 2 * LdotH * LdotH * 0.5;
	// �����Ă������̊g�U���˗�
	float FL = SchlickFresnel(1.0f, Fd90, NdotL);
	// �o�Ă������̊g�U���˗�
	float FV = SchlickFresnel(1.0f, Fd90, NdotV);
	// �����ďo�Ă����܂ł̊g�U���˗�
	float energyFactor = lerp(1.0f, 1.0f / 1.51f, 0.5);
	float Fd = FL * FV * energyFactor;

	// �g�U���ˍ�
	float3 diffuseColor = diffuseReflectance * Fd * baseColor * (1 - 0.5);

	// ���ʔ��ˍ�
	float3 specularColor = CookTorranceSpecular(NdotL, NdotV, NdotH, LdotH, baseColor);

	return diffuseColor + specularColor;
}

float4 Diffuse(float4 normal, float4 lightPos, float3 baseColor)
{
	return max(0, dot(normalize(normal.xyz), normalize(lightPos.xyz))) * float4(baseColor, 1);
}

float4 main(PS_IN input) : SV_Target
{
	// �ʂ̏���static�ϐ��ɑ�����A�֐�����Q�Ƃł���悤�ɂ���
	N = input.nor;
	// �ŏI�I��RGB
	float4 finalRGB = float4(0, 0, 0, 0);
	// ���_���王�_�ւ̕����x�N�g��
	float3 eyedir = normalize(CameraPos - input.posw.xyz);
	float3 lightdirMain = (LightPosMain.xyz - input.posw.xyz);
	float3 lightdirSub1 = (LightPos1.xyz - input.posw.xyz);
	float3 lightdirSub2 = (LightPos2.xyz - input.posw.xyz);
	float4 baseColor = Texture.Sample(Sampler, input.uv);

	float lightLengthMain = length(lightdirMain);
	float lightLengthSub1 = length(lightdirSub1);
	float lightLengthSub2 = length(lightdirSub2);

	// diffuse���v�Z��BRDF�̌��ʂƃ��C�g�F������
	// ���C�����C�g
	float4 diffuseColor = Diffuse(input.nor, LightPosMain, baseColor.xyz);
	finalRGB += (float4(BRDF(lightdirMain, eyedir, baseColor.xyz), 1) + diffuseColor) * 0.2;

	// �T�u���C�g�P
	diffuseColor = Diffuse(input.nor, LightPos1, baseColor.xyz);
	finalRGB += (float4(BRDF(lightdirSub1, eyedir, baseColor.xyz), 1) + diffuseColor) * (1.0 - smoothstep(10.0, 15.0, lightLengthSub1)) * 0.5;

	// �T�u���C�g�Q
	diffuseColor = Diffuse(input.nor, LightPos2, baseColor.xyz);
	finalRGB += (float4(BRDF(lightdirSub2, eyedir, baseColor.xyz), 1) + diffuseColor) * (1.0 - smoothstep(10.0, 15.0, lightLengthSub2)) * 0.5;

	//finalRGB += baseColor * 0.4;
	
	return float4(finalRGB.xyz, 1.0);
}
