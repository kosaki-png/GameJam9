#pragma once
#include "BaseLight.h"
#include <vector>
#include <array>

class LightGroup
{
public:
	LightGroup();
	~LightGroup();

	void Initialize();

	void Update();

	/// <summary>
	/// メインライト設定
	/// </summary>
	/// <param name="light">ライト</param>
	void SetMainLight(BaseLight* light);

	/// <summary>
	/// メインライト取得
	/// </summary>
	/// <returns></returns>
	BaseLight* GetMainLight();

	/// <summary>
	/// サブライト設定
	/// </summary>
	/// <param name="light">ライト</param>
	/// <param name="num">要素番号</param>
	void SetSubLight(int num, BaseLight* light);

	/// <summary>
	/// サブライト取得
	/// </summary>
	/// <param name="num">要素番号</param>
	/// <returns></returns>
	BaseLight* GetSubLight(int num);

private:
	BaseLight* mainLight = nullptr;
	std::array<BaseLight*, 2> subLights;

	/*ID3D11Buffer* mpStructuredBuffer;
	ID3D11ShaderResourceView* mpStructuredBufferSRV;*/
};