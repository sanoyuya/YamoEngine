#include "YFramework.h"
#include "MyGame.h"
#include"SceneFactory.h"
#include"SceneManager.h"
#include"imgui.h"
#include"LightManager.h"

void YFramework::Initialize()
{
	windowsApp = std::make_unique<WindowsApp>();//WindowsAppクラス読み込み
	windowsApp->CreatWindow(title.c_str(), window_width, window_height);//ウィンドウ作成

	fps = std::make_unique<FPS>();
	fps->Initialize();

	//DirectX初期化処理 ここから

	directX = DirectXBase::GetInstance();//DirectX_クラス読み込み
	directX->Initialize(windowsApp.get());

	//imGuiの初期化
	imGuiManager = ImGuiManager::GetInstance();
	imGuiManager->Initialize(windowsApp.get());

	//キー取得開始
	input = InputManager::GetInstance();
	input->Initialize(windowsApp.get());

	// オーディオの初期化
	audioManager = AudioManager::GetInstance();
	audioManager->Initialize();

	//DirectX初期化処理 ここまで

	//描画初期化処理ここから

	Camera::StaticInitialize(windowsApp.get());

	TextureManager::GetInstance()->StaticInitialize();

	SpriteCommon::Initialize();
	Sprite2D::StaticInitialize(windowsApp.get());

	Model::StaticInitialize();

	LightManager::StaticInitialize(DirectXBase::GetInstance()->GetDevice().Get());

	//描画初期化処理ここまで

	windowsApp->Appearance();

	//シーンファクトリーを生成し、マネージャにセット
	AbstractSceneFactory* sceneFactory = new SceneFactory();
	SceneManager::GetInstance()->SetSceneFactory(sceneFactory);

	postEffect = std::make_unique<PostEffect>();
	postEffect->Initialize();
}

void YFramework::Destroy()
{
	imGuiManager->Destroy();
	directX->Destroy();
	textureManager->Destroy();
	audioManager->Destroy();
}

void YFramework::Update()
{
	if (!windowsApp->MessageWindow())
	{
		endRequest = true;
	}

	input->Update();
	audioManager->Update();

	directX->SetClearColor();//背景色を設定 初期値(水色)
	directX->UpdateClear(windowsApp.get());
}

void YFramework::SetWindowData(const std::string& title_, const float width, const float height)
{
	wchar_t wtitle[256];

	MultiByteToWideChar(CP_ACP, 0, title_.c_str(), -1, wtitle, _countof(wtitle));

	title = wtitle;
	window_width = width;//横幅
	window_height = height;//縦幅
}

void YFramework::SetWindowColor(const myMath::Vector4& color)
{
	DirectXBase::GetInstance()->SetClearColor(color);
}

void YFramework::Run()
{
	//初期化処理
	Initialize();

	// ゲームループ
	while (true)
	{
		//更新処理
		imGuiManager->Begin();

		Update();

		imGuiManager->End();

		if (GetEndRequest())
		{
			break;
		}

		//描画処理
		Draw();
		postEffect->Draw();

#ifdef _DEBUG

		

#endif // DEBUG

		imGuiManager->Draw();//ALの評価課題出すまではこっち

		directX->UpdateEnd();

		//FPS制御
		fps->Update();
	}
	Destroy();
}

void YFramework::Draw()
{
}

bool& YFramework::GetEndRequest()
{
	return endRequest;
}