#include "GameScene.h"
#include "../Camera/TestCamera.h"
#include "../Scene/TestScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete camera;
	//delete light;
}

void GameScene::Initialize(Input* input)
{
	BaseScene::Initialize(input);

	camera = new TestCamera;
	camera->Initialize();
}

void GameScene::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		nextScene = new TestScene();
	}

	camera->Update();
}

void GameScene::Draw()
{
}

void GameScene::Finalize()
{
}
