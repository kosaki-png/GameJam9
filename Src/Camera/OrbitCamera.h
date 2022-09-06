#pragma once
#include "BaseCamera.h"
class OrbitCamera :
    public BaseCamera
{
public:
	OrbitCamera();
	~OrbitCamera();

	void Initialize() override;
	void Update() override;

public:
	void SetDistance(float dis) { distance = dis; }
	float GetDistance() { return distance; }

	void SetTheta(float theta) { this->theta = theta; }
	float GetTheta() { return theta; }

	void SetPhi(float phi) { this->phi = phi; }
	float GetPhi() { return phi; }

private:
	float distance = 7.0f;

	float theta = 45.0f;
	float phi = -90.0f;
};

