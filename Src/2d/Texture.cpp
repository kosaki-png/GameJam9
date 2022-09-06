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
        DirectGraphics::GetInstance()->GetDevice(),           //�f�o�C�X
        L"Res/Texture/white100x100.dds",    //�ǂݍ��މ摜�t�@�C���̏ꏊ�Ɩ��O
        nullptr,
        &shaderResourceView);     //�����ɓǂݍ��񂾃t�@�C���̏�񂪊i�[�����
}

void Texture::Draw()
{
    //Draw�̑O�ɌĂяo��
    spriteBatch->Begin();

    //Draw     
    spriteBatch->Draw(
        shaderResourceView,     //�ǂݍ��񂾉摜�t�@�C��
        DirectX::XMFLOAT2(0.0f, 0.0f)     //�摜�̍��W
    );

    //Draw�̌�ɌĂяo��
    spriteBatch->End();
}
