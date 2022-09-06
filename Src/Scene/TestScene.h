#pragma once
#include "BaseScene.h"
#include "../Element/LaptopPC.h"
#include "../2d/Texture.h"

class TestScene
	: public BaseScene
{
public:
	TestScene();
	~TestScene();

	void Initialize(Input* input) override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:
	ObjFile* testCube;
	ObjFile* testCube2;
	ObjFile* ground;

	LaptopPC* pc = nullptr;

	Texture* tex;
};

