#include "GameScene.h"
#include"PhysicsMath.h"
#include"ResultScene.h"

void GameScene::Initialize()
{
	input = InputManager::GetInstance();
	sceneManager = SceneManager::GetInstance();
	audioManager = AudioManager::GetInstance();
	score = Score::GetInstance();
	score->SetTimePos({1200,200});
	score->SetEnemyKillNumSpritePos({ 1200,500 });

	camera = std::make_unique<Camera>();
	camera->Initialize(true);
	cameraPos = { 0.0f,length,0.0000001f };

	player = std::make_unique<Player>();
	player->Initialize();

	//背景
	backSprite = std::make_unique<Sprite>();
	backSpriteTex = backSprite->LoadTexture("Resources/white1x1.png");
	backSprite->Sprite3DInitialize(backSpriteTex);
	backSpriteTrans.Initialize();
	backSpriteTrans.translation = { 0.0f,0.0f ,0.0f };
	backSpriteTrans.scale = { 90.0f,75.0f,0.0f };
	backSpriteTrans.rotation = { myMath::AX_PIF / 2,0.0f,0.0f };

	backLeftSprite = std::make_unique<Sprite>();
	backLeftSprite->Sprite3DInitialize(backSpriteTex);
	backLeftSpriteTrans.Initialize();
	backLeftSpriteTrans.translation = { 35.0f,0.0f ,0.0f };
	backLeftSpriteTrans.scale = { 20.0f,75.0f,0.0f };
	backLeftSpriteTrans.rotation = { myMath::AX_PIF / 2,0.0f,0.0f };

	backRightSprite = std::make_unique<Sprite>();
	backRightSprite->Sprite3DInitialize(backSpriteTex);
	backRightSpriteTrans.Initialize();
	backRightSpriteTrans.translation = { -35.0f,0.0f ,0.0f };
	backRightSpriteTrans.scale = { 20.0f,75.0f,0.0f };
	backRightSpriteTrans.rotation = { myMath::AX_PIF / 2,0.0f,0.0f };

	shadowSprite = std::make_unique<Sprite>();
	shadowSpriteTex = shadowSprite->LoadTexture("Resources/shadow.png");
	shadowSprite->Sprite3DInitialize(shadowSpriteTex);
	shadowSpriteTrans.Initialize();
	shadowSpriteTrans.translation = { 0.0f,0.0f ,0.0f };
	shadowSpriteTrans.scale = { 50.0f,75.0f ,0.0f };
	shadowSpriteTrans.rotation = { myMath::AX_PIF / 2,0.0f,0.0f };

	operation = std::make_unique<Sprite>();
	operationTex = operation->LoadTexture("Resources/operation.png");
	operation->Sprite2DInitialize(operationTex);


	coolTime = 30.0f;

	gameBGM = audioManager->LoadAudio("Resources/Sound/1~10.mp3");
	audioManager->ChangeVolume(gameBGM, 0.05f);
	audioManager->PlayWave(gameBGM);
}

void GameScene::Update()
{
	if (player->GetDeathFlag())
	{
		SceneChangeAnimation::GetInstance()->SetAnimationFlag(true);
	}

	if (SceneChangeAnimation::GetInstance()->GetAnimationTimer() == 30)
	{
		audioManager->StopWave(gameBGM);
		BaseScene* scene = new ResultScene();
		sceneManager->SetNextScene(scene);
	}

	if (input->KeyboardTriggerPush(DIK_R))
	{
		Reset();
		player->Reset();
	}
	EnemyDead();
	BackDiceDead();

	player->Update(camera.get());

	if (player->GetDeathAnimationFlag())
	{
		CamMove();
	}
	else
	{
		BackDiceUpdate();
		EnemyUpdate();
		score->Update();
	}

	camera->SetEye(cameraPos);
	camera->Update(true);

	backSpriteTrans.translation = { player->GetShakeAdd(),player->GetShakeAdd() ,0.0f };
	backSpriteTrans.Update();
	shadowSpriteTrans.translation = { player->GetShakeAdd(),player->GetShakeAdd() ,0.0f };
	shadowSpriteTrans.Update();
	backLeftSpriteTrans.translation = { 35.0f + player->GetShakeAdd(),player->GetShakeAdd() ,0.0f };
	backLeftSpriteTrans.Update();
	backRightSpriteTrans.translation = { -35.0f + player->GetShakeAdd(),player->GetShakeAdd() ,0.0f };
	backRightSpriteTrans.Update();
}

void GameScene::Draw()
{
	colorTime++;
	colorR = 0.25f + PhysicsMath::SimpleHarmonicMotion(colorTime, 0.025f, 1200.0f);
	colorG = 0.15f + PhysicsMath::SimpleHarmonicMotion(colorTime, 0.025f, 800.0f);
	colorB = 0.25f + PhysicsMath::SimpleHarmonicMotion(colorTime, 0.025f, 400.0f);
	backSprite->DrawSprite3D(camera.get(), backSpriteTrans, BillboardFlag::NonBillboard, { colorR,colorG,colorB,1.0f });

	shadowSprite->DrawSprite3D(camera.get(), shadowSpriteTrans);
	shadowSprite->DrawSprite3D(camera.get(), shadowSpriteTrans);

	BackDiceDraw();

	player->AttackRangeDraw(camera.get());

	EnemyDraw();

	backLeftSprite->DrawSprite3D(camera.get(), backLeftSpriteTrans, BillboardFlag::NonBillboard, { colorR,colorG,colorB,1.0f });
	backRightSprite->DrawSprite3D(camera.get(), backRightSpriteTrans, BillboardFlag::NonBillboard, { colorR,colorG,colorB,1.0f });

	player->Draw(camera.get());

	if (!player->GetDeathAnimationFlag())
	{
		score->TimeDraw({ 1.0f - colorR * 3,1.0f - colorG * 3,1.0f - colorB * 3,1.0f }, player->GetShakeAdd());
		score->EnemyKillNumDraw({ 1.0f - colorR * 3,1.0f - colorG * 3,1.0f - colorB * 3,1.0f }, player->GetShakeAdd());
		operation->DrawSprite2D({ 120.0f,500.0f });
	}
}

void GameScene::Destroy()
{
	backDices.clear();
	enemys.clear();
}

void GameScene::Rotation()
{
	if (input->KeyboardKeepPush(DIK_A))
	{

	}
	if (input->KeyboardKeepPush(DIK_D))
	{

	}
}

void GameScene::CamMove()
{
	zoomLengthX = player->GetTransform().translation.x;
	zoomLengthY = 15.0f;
	zoomLengthZ = player->GetTransform().translation.z;

	PhysicsMath::Complement(cameraPos.x, zoomLengthX, 30.0f);
	PhysicsMath::Complement(cameraPos.y, zoomLengthY, 30.0f);
	PhysicsMath::Complement(cameraPos.z, zoomLengthZ, 30.0f);

	camera->SetTarget({ cameraPos.x,0.0f ,cameraPos.z - 0.000001f });
}

void GameScene::BackDiceDead()
{
	backDices.remove_if([](std::unique_ptr<BackDice>& backDice) { return backDice->GetIsDead(); });
}

void GameScene::BackDiceUpdate()
{
	//背景サイコロの生成処理
	backDiceCoolTime++;
	if (backDiceCoolTime > 75)
	{
		myMath::Vector3 position = { static_cast<float>(myMath::GetRand(-20.0f,20.0f)),0.0f,static_cast<float>(myMath::GetRand(25.0f,30.0f)) };
		uint16_t num = static_cast<uint16_t>(myMath::GetRand(1, 6));

		//背景サイコロを生成し、初期化
		std::unique_ptr<BackDice> newBackDice = std::make_unique<BackDice>();
		newBackDice->Initialize(position, num);
		//背景サイコロを登録する
		backDices.push_back(std::move(newBackDice));

		backDiceCoolTime = 0.0f;
	}

	//背景サイコロの更新処理
	for (const std::unique_ptr<BackDice>& backDice : backDices)
	{
		backDice->Update(camera.get());
	}
}

void GameScene::BackDiceDraw()
{
	for (const std::unique_ptr<BackDice>& backDice : backDices)
	{
		backDice->Draw(camera.get(), { colorR ,colorG ,colorB ,1.0f });//背景のサイコロ
	}
}

void GameScene::EnemyDead()
{
	enemys.remove_if([](std::unique_ptr<Enemy>& enemy_) { return enemy_->GetIsDead(); });
	omnidirectionalBulletEnemys.remove_if([](std::unique_ptr<OmnidirectionalBulletEnemy>& omnidirectionalBulletEnemy) { return omnidirectionalBulletEnemy->GetIsDead(); });
}

void GameScene::EnemyUpdate()
{
	//敵の生成処理
	coolTime++;
	if (coolTime > 120)
	{
		myMath::Vector3 position = { static_cast<float>(myMath::GetRand(-23.0f,23.0f)),0.0f,static_cast<float>(myMath::GetRand(-23.0f,23.0f)) };
		//Enemyを生成し、初期化
		std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();
		newEnemy->Initialize(position);
		//Enemyを登録する
		enemys.push_back(std::move(newEnemy));

		coolTime = 0.0f;
	}

	//敵の更新処理
	for (const std::unique_ptr<Enemy>& enemy : enemys)
	{
		enemy->Update(camera.get(), player.get());
	}

	omnidirectionalBulletEnemyCoolTime++;
	if (omnidirectionalBulletEnemyCoolTime > 240)
	{
		myMath::Vector3 position = { static_cast<float>(myMath::GetRand(-23.0f,23.0f)),0.0f,static_cast<float>(myMath::GetRand(-23.0f,23.0f)) };
		//Enemyを生成し、初期化
		std::unique_ptr<OmnidirectionalBulletEnemy> newEnemy = std::make_unique<OmnidirectionalBulletEnemy>();
		newEnemy->Initialize(position);
		//Enemyを登録する
		omnidirectionalBulletEnemys.push_back(std::move(newEnemy));

		omnidirectionalBulletEnemyCoolTime = 0.0f;
	}

	//敵の更新処理
	for (const std::unique_ptr<OmnidirectionalBulletEnemy>& omnidirectionalBulletEnemy : omnidirectionalBulletEnemys)
	{
		omnidirectionalBulletEnemy->Update(camera.get(), player.get());
	}
}

void GameScene::EnemyDraw()
{
	for (const std::unique_ptr<Enemy>& enemy : enemys)
	{
		enemy->Draw(camera.get(), { colorR ,colorG ,colorB ,1.0f });//プレイヤーに向かってくる敵
	}

	for (const std::unique_ptr<OmnidirectionalBulletEnemy>& omnidirectionalBulletEnemy : omnidirectionalBulletEnemys)
	{
		omnidirectionalBulletEnemy->Draw(camera.get(), { colorR ,colorG ,colorB ,1.0f });//プレイヤーに向かってくる敵
	}
}

void GameScene::Reset()
{
	cameraPos = { 0.0f,length,0.000001f };
	backDices.clear();
	enemys.clear();
}