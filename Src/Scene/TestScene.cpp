#include "TestScene.h"

#include "../Camera/TestCamera.h"
#include "../Camera/OrbitCamera.h"

#include "../Light/TestLight.h"
#include "../Scene/GameScene.h"

TestScene::TestScene()
{
}

TestScene::~TestScene()
{
	delete camera;
}

void TestScene::Initialize(Input* input)
{
	BaseScene::Initialize(input);

	// �J����������
	{
		//camera = new TestCamera();
		camera = new OrbitCamera();
		camera->Initialize();
		camera->SetInput(input);
	}

	// ���C�g������
	{
		lightGroup = new LightGroup();

		// ���C�����C�g�ݒ�
		lightGroup->SetMainLight(new TestLight());
		// �T�u���C�g�ݒ�
		lightGroup->SetSubLight(0, new TestLight());	// �T�u���C�g�P
		lightGroup->SetSubLight(1, new TestLight());	// �T�u���C�g�Q

		// ���C�����C�g�i�^��O�j
		lightGroup->GetMainLight()->SetPosition({  0.0f, 10.0f,  5.0f });
		// �T�u���C�g�i�E���j
		lightGroup->GetSubLight(0)->SetPosition({  5.0f,  5.0f, -5.0f });
		// �T�u���C�g�i�����j
		lightGroup->GetSubLight(1)->SetPosition({ -5.0f,  5.0f, -5.0f });
	}

	// �I�u�W�F�N�g�����ݒ�
	{
		// �I�u�W�F�N�g����
		{
			testCube = new ObjFile();
			//testCube2 = new ObjFile();
			ground = new ObjFile();

			pc = new LaptopPC();
		}

		// ���f���̃��[�h
		{
			testCube->Load(
				"Res/Model/Desk/Desk.obj",
				DirectGraphics::GetInstance()->GetDevice(),
				DirectGraphics::GetInstance()->GetVertexShader());

			/*testCube2->Load(
				"Res/Model/PC/PC_Keyboard/PC_Keyboard.obj",
				DirectGraphics::GetInstance()->GetDevice(),
				DirectGraphics::GetInstance()->GetVertexShader());*/

			ground->Load(
				"Res/Model/Ground/ground.obj",
				DirectGraphics::GetInstance()->GetDevice(),
				DirectGraphics::GetInstance()->GetVertexShader());
		}

		// �I�u�W�F�N�g�̏�����
		{
			pc->Initialize();
		}
	}
	
	// �I�u�W�F�N�g�}�l�[�W���[�ɒǉ�
	{
		objMng = ObjectManager::GetInstance();

		objMng->AddObject(testCube);
		/*objMng->AddObject(testCube2);*/
		objMng->AddObject(ground);
	}

	/*tex = new Texture();
	tex->Initialize();
	tex->LoadTexture();*/
}

void TestScene::Update()
{
	if (input->TriggerKey(DIK_SPACE))
	{
		nextScene = new GameScene();
	}

	// PC�̊J����
	if (input->TriggerKey(DIK_RETURN))
	{
		pc->ChangeOpen();
	}

	// �X�N���[�����[�h�؂�ւ�
	if (input->TriggerKey(DIK_F1))
	{
		DirectGraphics::GetInstance()->SetScreenMode(!DirectGraphics::GetInstance()->GetScreenMode());
	}

	// �J�����X�V
	{
		static float a = 0;
		a++;
		//camera->SetRotation({ 0,a,0 });
	}

	if (input->TriggerMouseLeft())
	{
		pc->ChangeOpen();
	}

	//testCube->SetPosition({ 5, 0, 0 });
	ground->SetPosition(Vector3(0.0f, -5.0f, 0.0f));
	pc->SetPosition({ -0.8f, 2.1f, -0.5f });

	pc->Update();

	camera->Update();
	objMng->Update();

}

void TestScene::Draw()
{
	objMng->Render(DirectGraphics::GetInstance());

	pc->Draw(DirectGraphics::GetInstance());

	//tex->Draw();
}

void TestScene::Finalize()
{
	objMng->Relese();
}
