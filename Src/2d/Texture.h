#pragma once
#include <d3d11.h>
#include <iostream>
#include <directxmath.h>
#include <SpriteBatch.h>
#include <DDSTextureLoader.h>

class Texture
{
public:
	Texture();
	~Texture();

	void Initialize();
	void LoadTexture();
	void Draw();

private:
	DirectX::SpriteBatch* spriteBatch; 
	ID3D11ShaderResourceView* shaderResourceView;     //読み込んだ画像ファイルの保存先
};

