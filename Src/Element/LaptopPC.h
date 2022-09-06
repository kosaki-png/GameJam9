#pragma once
#include "Element.h"

class LaptopPC
	: public Element
{
public:
	LaptopPC();
	~LaptopPC();

	void Initialize() override;
	void Update() override;
	void Draw(DirectGraphics* graphics) override;

	void ChangeOpen();

private:
	ObjFile* model_window;
	ObjFile* model_keyboard;

	bool isOpen = true;
	float openRad = 0.0f;
};

