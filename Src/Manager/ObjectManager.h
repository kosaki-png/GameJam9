#pragma once
#include "../3d/ObjFile.h"

class ObjectManager
{
public:
	static ObjectManager* GetInstance();

	void AddObject(ObjFile* object);

	ObjFile* GetObjFile(int num);

	void Update();

	void Render(DirectGraphics* graphics);

	void Relese();

	void Destroy();

private:
	ObjectManager() = default;
	~ObjectManager() = default;

	static ObjectManager* instance;

	std::vector<ObjFile*> objects;

};

