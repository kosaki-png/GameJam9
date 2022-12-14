#pragma once
#include "../Scene/BaseScene.h"

class SceneManager
{
public:
	static SceneManager* GetInstance();

	/// <summary>
	/// 現在のシーン取得
	/// </summary>
	/// <returns></returns>
	BaseScene* GetNowScene();

	void Initialize(Input* input);
	void Update();
	void Draw();
	void Finalize();

	void Destroy();

private:
	static SceneManager* instance;

	SceneManager() = default;
	~SceneManager() = default;

	BaseScene* nowScene = nullptr;

	Input* input = nullptr;

};