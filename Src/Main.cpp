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
	// �E�B���h�E�̐ݒ�
	Window* window;
	window = new Window("Game", 1920, 1080);

	// �E�B���h�E���쐬
	if (window->Create() == false)
	{
		return 0;
	}

	if (DirectGraphics::GetInstance()->Init() == false)
	{
		return 0;
	}

	// �C���v�b�g����
	Input* input = nullptr;
	input = new Input(window);

	// �V�[������
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

	// ���C�����[�v
	while (true)
	{
		MSG msg;

		// �I�����b�Z�[�W
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
			// �X�V
			sceneMng->Update();
			DirectGraphics::GetInstance()->SetTransform(sceneMng->GetNowScene()->GetCamera(), sceneMng->GetNowScene()->GetLightList());

			//DirectGraphics::GetInstance()->SetUpTransform();

			DirectGraphics::GetInstance()->StartRendering();

			DirectGraphics::GetInstance()->SetUpContext();

			// �`��
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

	// �V�[���J��
	sceneMng->Finalize();
	sceneMng->Destroy();
	delete window;

	DirectGraphics::GetInstance()->Release();

	return 0;
}
