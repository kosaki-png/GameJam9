#include "LaptopPC.h"

LaptopPC::LaptopPC()
{
}

LaptopPC::~LaptopPC()
{
	delete model_window;
	delete model_keyboard;
}

void LaptopPC::Initialize()
{
	// オブジェクト
	{
		// オブジェクト生成
		{
			model_window = new ObjFile();
			model_keyboard = new ObjFile();
		}

		// モデル読み込み
		{
			model_window->Load(
				"Res/Model/PC/PC_Window/PC_Window.obj",
				DirectGraphics::GetInstance()->GetDevice(),
				DirectGraphics::GetInstance()->GetVertexShader());

			model_keyboard->Load(
				"Res/Model/PC/PC_Keyboard/PC_Keyboard.obj",
				DirectGraphics::GetInstance()->GetDevice(),
				DirectGraphics::GetInstance()->GetVertexShader());
		}

		// オブジェクト初期設定
		{
			model_window->SetPosition({ 0.0f * scale.X, 0.017f * scale.Y, -0.31f * scale.Z });

			model_keyboard->RegistChildObj(model_window);
		}
	}
	
}

void LaptopPC::Update()
{
	if (isOpen)
	{
		if (openRad > 0)
		{
			openRad--;
		}
	}
	else
	{
		if (openRad < 90)
		{
			openRad++;
		}
	}

	//rotation.X++;
	/*rotation.Y++;
	rotation.Z++;*/
	//model_keyboard->SetScale({ 3,3,3 });

	// 座標セット
	model_keyboard->SetPosition(position);

	// 角度セット
	model_window->SetRotation({ openRad, 0.0f, 0.0f });
	model_keyboard->SetRotation(rotation);

	model_window->Update();
	model_keyboard->Update();
}

void LaptopPC::Draw(DirectGraphics* graphics)
{
	model_window->Render(graphics);
	model_keyboard->Render(graphics);
}

void LaptopPC::ChangeOpen()
{
	isOpen = !isOpen;
}
