#include "TestCamera.h"

TestCamera::TestCamera()
{
}

TestCamera::~TestCamera()
{
}

void TestCamera::Initialize()
{
	BaseCamera::Initialize();
}

void TestCamera::Update()
{
	if (input)
	{
		auto mouseVel = input->GetMouseVel();

		rotation.X += mouseVel.X * 0.05f;
		rotation.Y += mouseVel.Y * 0.05f;

		//rotation.Y++;
		SetRotation(rotation);

		ShowCursor(false);
		SetCursorPos(1920 / 2, 1080 / 2);
	}

	// çXêV
	BaseCamera::Update();
}
