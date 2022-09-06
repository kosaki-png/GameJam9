#pragma once
#include "BaseScene.h"

class GameScene
	: public BaseScene
{
public:
	GameScene();
	~GameScene();

	void Initialize(Input* input) override;

	void Update() override;

	void Draw() override;

	void Finalize() override;

private:


};

