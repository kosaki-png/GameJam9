#include "Texture.h"
#include "../Base/DirectGraphics.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

void Texture::Initialize()
{
	spriteBatch = new DirectX::SpriteBatch(DirectGraphics::GetInstance()->GetContext());
}

void Texture::LoadTexture()
{
    DirectX::CreateDDSTextureFromFile(
        DirectGraphics::GetInstance()->GetDevice(),           //デバイス
        L"Res/Texture/white100x100.dds",    //読み込む画像ファイルの場所と名前
        nullptr,
        &shaderResourceView);     //ここに読み込んだファイルの情報が格納される
}

void Texture::Draw()
{
    //Drawの前に呼び出す
    spriteBatch->Begin();

    //Draw     
    spriteBatch->Draw(
        shaderResourceView,     //読み込んだ画像ファイル
        DirectX::XMFLOAT2(0.0f, 0.0f)     //画像の座標
    );

    //Drawの後に呼び出す
    spriteBatch->End();
}
