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
	// メインライト初期化
	if (mainLight)
	{
		mainLight->Initialize();
	}

	// サブライト初期化
	for (int i = 0; i < 2; i++)
	{
		// nullでないなら
		if (subLights[i])
		{
			subLights[i]->Initialize();
		}
	}

	{
		//D3D11_BUFFER_DESC desc = {};
	////StructuredBufferとして扱うときはMiscFlagにD3D11_RESOURCE_MISC_BUFFER_STRUCTUREDを指定する必要がある
	//desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	////構造体のサイズも一緒に設定すること
	//desc.StructureByteStride = sizeof(LightStatus);
	////後は他のバッファと同じ
	//desc.ByteWidth = sizeof(LightStatus);
	//desc.Usage = D3D11_USAGE_DEFAULT;
	//desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	////初期データの設定
	//LightStatus info;
	//info.light_pos = Vector3(0, 0, 0);
	//info.attenuation = 0;
	//info.light_color = Vector3(0, 0, 0);
	//info.lightfov = 0;
	//info.light_dir = Vector3(0, 0, 0);
	//D3D11_SUBRESOURCE_DATA initData;
	//initData.pSysMem = &info;
	//initData.SysMemPitch = sizeof(info);
	////バッファ作成
	//auto hr = DirectGraphics::GetInstance()->GetDevice()->CreateBuffer(&desc, &initData, &mpStructuredBuffer);
	//if (FAILED(hr)) {
	//	assert(0);
	//}
	////シェーダリソースビューも作る必要がある
	////また、Formatは必ずDXGI_FORMAT_UNKNOWNにしないといけない
	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	//srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	//srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
	//srvDesc.Buffer.FirstElement = 0;
	//srvDesc.Buffer.NumElements = 1;
	//// 次のコードでも設定出来る
	////srvDesc.Buffer.ElementOffset = 0;
	////srvDesc.Buffer.ElementWidth = 1;
	//// ViewDimensionにD3D11_SRV_DIMENSION_BUFFEREXを指定してもOK
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
	// メインライト更新
	if (mainLight)
	{
		mainLight->Update();
	}

	// サブライト更新
	for (int i = 0; i < 2; i++)
	{
		// nullでないなら
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
