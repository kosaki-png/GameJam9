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
	ID3D11ShaderResourceView* shaderResourceView;     //“Ç‚İ‚ñ‚¾‰æ‘œƒtƒ@ƒCƒ‹‚Ì•Û‘¶æ
};

