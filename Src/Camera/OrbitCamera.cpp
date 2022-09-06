#include "OrbitCamera.h"

OrbitCamera::OrbitCamera()
{
}

OrbitCamera::~OrbitCamera()
{
}

void OrbitCamera::Initialize()
{
	BaseCamera::Initialize();
}

void OrbitCamera::Update()
{
	phi += 0.2f;

	float angleX = phi * PI / 180;
	float angleY = theta * PI / 180;

	float sinX = sinf(angleX);
	float cosX = cosf(angleX);
	float sinY = sinf(angleY);
	float cosY = cosf(angleY);

	SetEye(Vector3(cosX * cosY * distance,
				   sinY		   * distance,
				   sinX * cosY * distance));

	BaseCamera::Update();
}
