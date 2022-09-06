#include "LightGroup.h"
#include "../Base/DirectGraphics.h"

LightGroup::LightGroup()
{
}

LightGroup::~LightGroup()
{
	delete mainLight;
}

void LightGroup::Initialize()
{
	// ���C�����C�g������
	if (mainLight)
	{
		mainLight->Initialize();
	}

	// �T�u���C�g������
	for (int i = 0; i < 2; i++)
	{
		// null�łȂ��Ȃ�
		if (subLights[i])
		{
			subLights[i]->Initialize();
		}
	}

	{
		//D3D11_BUFFER_DESC desc = {};
	////StructuredBuffer�Ƃ��Ĉ����Ƃ���MiscFlag��D3D11_RESOURCE_MISC_BUFFER_STRUCTURED���w�肷��K�v������
	//desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	////�\���̂̃T�C�Y���ꏏ�ɐݒ肷�邱��
	//desc.StructureByteStride = sizeof(LightStatus);
	////��͑��̃o�b�t�@�Ɠ���
	//desc.ByteWidth = sizeof(LightStatus);
	//desc.Usage = D3D11_USAGE_DEFAULT;
	//desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	////�����f�[�^�̐ݒ�
	//LightStatus info;
	//info.light_pos = Vector3(0, 0, 0);
	//info.attenuation = 0;
	//info.light_color = Vector3(0, 0, 0);
	//info.lightfov = 0;
	//info.light_dir = Vector3(0, 0, 0);
	//D3D11_SUBRESOURCE_DATA initData;
	//initData.pSysMem = &info;
	//initData.SysMemPitch = sizeof(info);
	////�o�b�t�@�쐬
	//auto hr = DirectGraphics::GetInstance()->GetDevice()->CreateBuffer(&desc, &initData, &mpStructuredBuffer);
	//if (FAILED(hr)) {
	//	assert(0);
	//}
	////�V�F�[�_���\�[�X�r���[�����K�v������
	////�܂��AFormat�͕K��DXGI_FORMAT_UNKNOWN�ɂ��Ȃ��Ƃ����Ȃ�
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	//srvDesc.Buffer.FirstElement = 0;
	//srvDesc.Buffer.NumElements = 1;
	//// ���̃R�[�h�ł��ݒ�o����
	////srvDesc.Buffer.ElementOffset = 0;
	////srvDesc.Buffer.ElementWidth = 1;
	//// ViewDimension��D3D11_SRV_DIMENSION_BUFFEREX���w�肵�Ă�OK
	////srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	////srvDesc.BufferEx.FirstElement = 0;
	////srvDesc.BufferEx.Flags = 0;
	////srvDesc.BufferEx.NumElements = 1;
	//hr = DirectGraphics::GetInstance()->GetDevice()->CreateShaderResourceView(mpStructuredBuffer, &srvDesc, &this->mpStructuredBufferSRV);
	//if (FAILED(hr)) {
	//	assert(0);
	//}
	}
}

void LightGroup::Update()
{
	// ���C�����C�g�X�V
	if (mainLight)
	{
		mainLight->Update();
	}

	// �T�u���C�g�X�V
	for (int i = 0; i < 2; i++)
	{
		// null�łȂ��Ȃ�
		if (subLights[i])
		{
			subLights[i]->Update();
		}
	}
}

void LightGroup::SetMainLight(BaseLight* light)
{
	mainLight = light;
}

BaseLight* LightGroup::GetMainLight()
{
	return mainLight;
}

void LightGroup::SetSubLight(int num, BaseLight* light)
{
	subLights[num] = light;
}

BaseLight* LightGroup::GetSubLight(int num)
{
	return subLights[num];
}
