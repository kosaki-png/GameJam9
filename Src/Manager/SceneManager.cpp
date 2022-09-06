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

	// �ŏ��̃V�[��
	nowScene = new TestScene();
	//nowScene = new GameScene();

	nowScene->Initialize(input);
}

void SceneManager::Update()
{
	BaseScene* nextScene = nowScene->GetNextScene();

	// �l����������
	if (nextScene)
	{
		// �I������
		nowScene->Finalize();

		// ���̃V�[�������
		delete nowScene;

		// �u������
		nowScene = nextScene;
		nowScene->Initialize(input);
	}

	// �V�[���X�V
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
