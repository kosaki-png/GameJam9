#include "SceneManager.h"

#include "../Scene/TestScene.h"
#include "../Scene/GameScene.h"

SceneManager* SceneManager::instance = nullptr;

SceneManager* SceneManager::GetInstance()
{
	if (!instance)
	{
		instance = new SceneManager();
	}
	return instance;
}

BaseScene* SceneManager::GetNowScene()
{
	return nowScene;
}

void SceneManager::Initialize(Input* input)
{
	this->input = input;

	// 最初のシーン
	nowScene = new TestScene();
	//nowScene = new GameScene();

	nowScene->Initialize(input);
}

void SceneManager::Update()
{
	BaseScene* nextScene = nowScene->GetNextScene();

	// 値が入ったら
	if (nextScene)
	{
		// 終了処理
		nowScene->Finalize();

		// 元のシーンを解放
		delete nowScene;

		// 置き換え
		nowScene = nextScene;
		nowScene->Initialize(input);
	}

	// シーン更新
	nowScene->Update();
}

void SceneManager::Draw()
{
	nowScene->Draw();
}

void SceneManager::Finalize()
{
	nowScene->Finalize();
}

void SceneManager::Destroy()
{
	delete nowScene;
	delete instance;
}
