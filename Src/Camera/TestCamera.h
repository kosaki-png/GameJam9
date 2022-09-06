#pragma once
#include "BaseCamera.h"

class TestCamera
	: public BaseCamera
{
public:
	TestCamera();
	~TestCamera();

	void Initialize() override;
	void Update() override;

private:

};

