#include "ObjectManager.h"

ObjectManager* ObjectManager::instance = nullptr;

ObjectManager* ObjectManager::GetInstance()
{
    if (!instance) {
        instance = new ObjectManager();
    }
    return instance;
}

void ObjectManager::AddObject(ObjFile* object)
{
    objects.push_back(object);
}

ObjFile* ObjectManager::GetObjFile(int num)
{
    return objects[num];
}

void ObjectManager::Update()
{
    for (auto x : objects)
    {
        x->Update();
    }
}

void ObjectManager::Render(DirectGraphics* graphics)
{
    for (auto x : objects)
    {
        x->Render(graphics);
    }
}

void ObjectManager::Relese()
{
    for (int i = objects.size() - 1; i > 0; i--)
    {
        delete objects[i];
    }
    objects.clear();
}

void ObjectManager::Destroy()
{
    delete instance;
}
