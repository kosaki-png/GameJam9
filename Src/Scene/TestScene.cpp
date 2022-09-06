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

	// カメラ初期化
	{
		//camera = new TestCamera();
		camera = new OrbitCamera();
		camera->Initialize();
		camera->SetInput(input);
	}

	// ライト初期化
	{
		lightGroup = new LightGroup();

		// メインライト設定
		lightGroup->SetMainLight(new TestLight());
		// サブライト設定
		lightGroup->SetSubLight(0, new TestLight());	// サブライト１
		lightGroup->SetSubLight(1, new TestLight());	// サブライト２

		// メインライト（真上前）
		lightGroup->GetMainLight()->SetPosition({  0.0f, 10.0f,  5.0f });
		// サブライト（右上後）
		lightGroup->GetSubLight(0)->SetPosition({  5.0f,  5.0f, -5.0f });
		// サブライト（左上後）
		lightGroup->GetSubLight(1)->SetPosition({ -5.0f,  5.0f, -5.0f });
	}

	// オブジェクト初期設定
	{
		// オブジェクト生成
		{
			testCube = new ObjFile();
			//testCube2 = new ObjFile();
			ground = new ObjFile();

			pc = new LaptopPC();
		}

		// モデルのロード
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

		// オブジェクトの初期化
		{
			pc->Initialize();
		}
	}
	
	// オブジェクトマネージャーに追加
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

	// PCの開け閉め
	if (input->TriggerKey(DIK_RETURN))
	{
		pc->ChangeOpen();
	}

	// スクリーンモード切り替え
	if (input->TriggerKey(DIK_F1))
	{
		DirectGraphics::GetInstance()->SetScreenMode(!DirectGraphics::GetInstance()->GetScreenMode());
	}

	// カメラ更新
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
