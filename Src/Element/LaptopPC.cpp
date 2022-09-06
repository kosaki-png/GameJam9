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
	// �I�u�W�F�N�g
	{
		// �I�u�W�F�N�g����
		{
			model_window = new ObjFile();
			model_keyboard = new ObjFile();
		}

		// ���f���ǂݍ���
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

		// �I�u�W�F�N�g�����ݒ�
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

	// ���W�Z�b�g
	model_keyboard->SetPosition(position);

	// �p�x�Z�b�g
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
