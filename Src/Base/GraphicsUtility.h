#ifndef GRAPHICS_UTILITY_H_
#define GRAPHICS_UTILITY_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <string>

struct ConstantBuffer
{
	DirectX::XMFLOAT4X4 World;
	DirectX::XMFLOAT4X4 View;
	DirectX::XMFLOAT4X4 Projection;
	DirectX::XMFLOAT4	CameraPos;
	DirectX::XMFLOAT4	MaterialAmbient;
	DirectX::XMFLOAT4	MaterialDiffuse;
	DirectX::XMFLOAT4	MaterialSpecular;
	DirectX::XMFLOAT4	LightPosMain;
	DirectX::XMFLOAT4	LightPos1;
	DirectX::XMFLOAT4	LightPos2;
	DirectX::XMFLOAT4   LightColorMain;
	DirectX::XMFLOAT4   LightColor1;
	DirectX::XMFLOAT4   LightColor2;
};


struct Vector3
{
	Vector3()
	{
		this->X = 0.0f;
		this->Y = 0.0f;
		this->Z = 0.0f;
	}

	Vector3(float x, float y, float z)
	{
		this->X = x;
		this->Y = y;
		this->Z = z;
	}

	float X;
	float Y;
	float Z;
};

struct Vector2
{
	Vector2()
	{
		this->X = 0.0f;
		this->Y = 0.0f;
	}

	Vector2(float x, float y)
	{
		this->X = x;
		this->Y = y;
	}

	float X;
	float Y;
};

struct Color
{
	float Red;
	float Green;
	float Blue;
	float Alpha;

	Color(float red, float green, float blue, float alpha)
	{
		Red = red;
		Green = green;
		Blue = blue;
		Alpha = alpha;
	}

	Color()
	{
		Red = Green = Blue = Alpha = 1.0f;
	}

};

//=====================================================================//
//! ポリゴン出力用カスタムバーテックス構造体
//=====================================================================//
struct CustomVertex
{
	Vector3 Position;		// 座標(x, y, z)
	Vector3 Normal;			// 法線
	Vector2 TexturePos;		// テクスチャ座標(u, v)
};


struct ObjMaterial
{
	float Ambient[4];
	float Diffuse[4];
	float Specular[4];
	std::string TextureKeyWord;
	std::string TextureName;
};

//複数ライト用構造体
struct LightStatus
{
	Vector3 light_pos;		// ライトの座標
	float   attenuation;	// ライトの減衰
	Vector3 light_color;	// ライトの色
	float   lightfov;		// ライトの照らす範囲
	Vector3 light_dir;		// ライトの向き
};

#endif
