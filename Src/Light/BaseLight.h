#pragma once
#include "../Base/GraphicsUtility.h"

class BaseLight
{
public:
	BaseLight() = default;
	~BaseLight() = default;

	virtual void Initialize();
	virtual void Update();

	void SetPosition(Vector3 position) { this->position = position; }
	Vector3 GetPosition() { return position; }

	void SetColor(DirectX::XMFLOAT4 color) { this->color = color; }
	DirectX::XMFLOAT4 GetColor() { return color; }

protected:
	Vector3 position = {0,0,0};
	DirectX::XMFLOAT4 color = { 1,1,1,1 };

	LightStatus lightStatus;
};

