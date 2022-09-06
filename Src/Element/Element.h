#pragma once
#include "../3d/ObjFile.h"

class Element
{
public:
	Element() = default;
	~Element() = default;

	virtual void Initialize();
	virtual void Update();
	virtual void Draw(DirectGraphics* graphics);

	// setter, getter
	void SetPosition(Vector3 position) { this->position = position; }
	Vector3 GetPosition() { return position; }

	void SetRotation(Vector3 rotation) { this->rotation = rotation; }
	Vector3 GetRotation() { return rotation; }

	void SetScale(Vector3 scale) { this->scale = scale; }
	Vector3 GetScale() { return scale; }

protected:
	Vector3 position = { 0,0,0 };
	Vector3 rotation = { 0,0,0 };
	Vector3 scale = { 1,1,1 };

};

