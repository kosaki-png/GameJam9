#pragma once
#include "../3d/ObjFile.h"
#include "../Camera/BaseCamera.h"

#include "../Manager/ObjectManager.h"

#include "../Light/LightGroup.h"

class BaseScene
{
public:
	virtual void Initialize(Input* input);

	virtual void Update();

	virtual void Draw();

	virtual void Finalize();

	BaseScene* GetNextScene() { return nextScene; }
	BaseCamera* GetCamera() { return camera; }
	LightGroup* GetLightList() { return lightGroup; }

protected:
	BaseScene* nextScene = nullptr;

	Input* input = nullptr;

	BaseCamera* camera = nullptr;

	ObjectManager* objMng = nullptr;

	LightGroup* lightGroup = nullptr;
};

