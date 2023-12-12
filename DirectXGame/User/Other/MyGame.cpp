#include "MyGame.h"
#include"SceneManager.h"
#include"SceneChangeAnimation.h"
#include"Enemy.h"
#include"HitEffect.h"
#include"Pose.h"
#include"Retention.h"
#include"GameHeader.h"
#include"EnemyLocationSprite.h"
#include"TutorialSkip.h"
#include"Text.h"

void MyGame::Initialize()
{
#ifdef _DEBUG
	YFramework::SetWindowData("AirBattle(Debug)");
#else
	YFramework::SetWindowData("AirBattle");
#endif
	YFramework::SetWindowColor();

	//基底クラスの初期化処理
	YFramework::Initialize();

	GameHeader::SetApp(windowsApp_.get());

	//ポストエフェクトの初期化
	postEffect_ = std::make_unique<MultiTexturePostEffect>();
	postEffect_->Initialize(windowsApp_.get());

	LoadAsset();

#ifdef _DEBUG
	//シーンマネージャーに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("TITLE");
#else
	//シーンマネージャーに最初のシーンをセット
	SceneManager::GetInstance()->ChangeScene("EngineOP");
#endif
}

void MyGame::Destroy()
{
	SceneManager::GetInstance()->Destroy();

	//基底クラスの終了処理
	YFramework::Destroy();
}

void MyGame::Update()
{
	//基底クラスの更新処理
	YFramework::Update();

	//シーンチェンジアニメーションの更新処理
	SceneChangeAnimation::GetInstance()->Update();

	//シーンマネージャーの更新処理
	SceneManager::GetInstance()->Update();
}

void MyGame::SceneDraw()
{
	postEffect_->PreDrawScene(windowsApp_.get());
	SceneManager::GetInstance()->Draw();
	postEffect_->PostDrawScene();
}

void MyGame::PostEffectDraw()
{
	postEffect_->Draw();
}

void MyGame::LoadAsset()
{
	SceneChangeAnimation::GetInstance()->StaticInitialize();
	Enemy::StaticInitialize();
	HitEffect::StaticInitialize();
	Pose::GetInstance()->Initialize();
	EnemyLocationSprite::StaticInitialize();
	Text::LoadAsset();
	TutorialSkip::LoadAsset();
}