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
	/// ���C�����C�g�ݒ�
	/// </summary>
	/// <param name="light">���C�g</param>
	void SetMainLight(BaseLight* light);

	/// <summary>
	/// ���C�����C�g�擾
	/// </summary>
	/// <returns></returns>
	BaseLight* GetMainLight();

	/// <summary>
	/// �T�u���C�g�ݒ�
	/// </summary>
	/// <param name="light">���C�g</param>
	/// <param name="num">�v�f�ԍ�</param>
	void SetSubLight(int num, BaseLight* light);

	/// <summary>
	/// �T�u���C�g�擾
	/// </summary>
	/// <param name="num">�v�f�ԍ�</param>
	/// <returns></returns>
	BaseLight* GetSubLight(int num);

private:
	BaseLight* mainLight = nullptr;
	std::array<BaseLight*, 2> subLights;

	/*ID3D11Buffer* mpStructuredBuffer;
	ID3D11ShaderResourceView* mpStructuredBufferSRV;*/
};