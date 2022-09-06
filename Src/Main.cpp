#include <Windows.h>
#include <DirectXMath.h>
#include <vector>
#include <time.h>
#include <mmsystem.h>
#include "Base/Window.h"
#include "Base/DirectGraphics.h"
#include "3d/ObjFile.h"

#include "Manager/SceneManager.h"
#include "Scene/TestScene.h"

#pragma comment(lib, "winmm.lib")

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmpLine,
	INT       nCmdShow)
{
	// ウィンドウの設定
	Window* window;
	window = new Window("Game", 1920, 1080);

	// ウィンドウを作成
	if (window->Create() == false)
	{
		return 0;
	}

	if (DirectGraphics::GetInstance()->Init() == false)
	{
		return 0;
	}

	// インプット生成
	Input* input = nullptr;
	input = new Input(window);

	// シーン生成
	SceneManager* sceneMng = nullptr;
	sceneMng = SceneManager::GetInstance();
	sceneMng->Initialize(input);

	//DirectGraphics::GetInstance()->SetTransform(sceneMng->GetNowScene()->GetCamera(), sceneMng->GetNowScene()->GetLightList());

	Texture* tex = new Texture();
	tex->Initialize();
	tex->LoadTexture();

	timeBeginPeriod(1);

	DWORD current = timeGetTime();
	DWORD before = current;

	// メインループ
	while (true)
	{
		MSG msg;

		// 終了メッセージ
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// 更新
			sceneMng->Update();
			DirectGraphics::GetInstance()->SetTransform(sceneMng->GetNowScene()->GetCamera(), sceneMng->GetNowScene()->GetLightList());

			//DirectGraphics::GetInstance()->SetUpTransform();

			DirectGraphics::GetInstance()->StartRendering();

			DirectGraphics::GetInstance()->SetUpContext();

			// 描画
			sceneMng->Draw();
			tex->Draw();

			DirectGraphics::GetInstance()->FinishRendering();

			

			while (current - before < 17)
			{
				Sleep(current - before);

				current = timeGetTime();
			}

			before = current;
		}
	}

	timeEndPeriod(1);

	// シーン開放
	sceneMng->Finalize();
	sceneMng->Destroy();
	delete window;

	DirectGraphics::GetInstance()->Release();

	return 0;
}
