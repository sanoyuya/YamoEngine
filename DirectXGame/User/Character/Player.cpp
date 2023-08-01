#include "Player.h"
#include"PhysicsMath.h"
#include"ColliderManager.h"
#include<imgui.h>

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize()
{
	input_ = InputManager::GetInstance();

	//レティクルの初期化
	reticle_ = std::make_unique<Reticle>();
	reticle_->Initialize();

	//プレイヤーの初期化
	player_ = std::make_unique<Model>();
	playerTex_ = player_->CreateObjModel("Resources/F-35E");
	player_->SetModel(playerTex_);
	playerTrans_.Initialize();
	playerTrans_.translation = { 0.0f,-reticle_->GetReticleLimit() / 3,10.0f };
	playerTrans_.scale = { 0.5f,0.5f,0.5f };
	cameraTrans_.Initialize();

	//HPバーの初期化
	hpBar_ = std::make_unique<HPBar>();
	hpBar_->Initialize(maxHp_, { 100.0f,100.0f });

	// パーティクル生成
	smokeEmitter_ = std::make_unique<PlayerEngineSmokeParticleEmitter>();
	smokeEmitter_->Initialize();
	smokeTrans_.Initialize();

	deathParticleEmitter_ = std::make_unique<EnemyDeathParticleEmitter>();
	deathParticleEmitter_->Initialize();
	deathParticleEmitterTrans_.Initialize();
}

void Player::Update(Camera* camera)
{
	cameraTrans_.matWorld = camera->GetMatView();
	playerTrans_.parent = &cameraTrans_;

	//HPバーの更新
	hpBar_->Update(hp_);

	if (hp_ <= 0)
	{
		if (animationFlag_ == false)
		{
			deathParticleEmitter_->Create(playerTrans_.parentToTranslation);
			animationFlag_ = true;
		}
		else
		{
			deathAnimationTimer++;
			if (deathAnimationTimer >= 60.0f)
			{
				deathFlag_ = true;
			}
		}
	}
	else
	{
		reticle_->Update(camera);
		Move();
	}

	playerTrans_.TransUpdate(camera);

	//ローカルの正面ベクトル
	directionVector_ = reticle_->GetTransform().translation - playerTrans_.translation;
	//正規化
	directionVector_.normalization();

	//ワールドの正面ベクトル
	parentToDirectionVector_ = reticle_->GetTransform().parentToTranslation - playerTrans_.parentToTranslation;
	//正規化
	parentToDirectionVector_.normalization();

	Rotation(camera);

	BulletUpdate(camera);

	SmokeUpdate(camera);

	if (animationFlag_ == true)
	{
		deathParticleEmitter_->Update(camera);
	}
}

void Player::Draw(Camera* camera)
{
	//弾の描画
	BulletDraw();

	if (hp_ > 0)
	{
		//レティクルの描画
		reticle_->Draw(camera);
		//プレイヤーのモデル描画
		player_->DrawModel(&playerTrans_);
		//機体のエンジンから火が出るパーティクルの描画
		smokeEmitter_->Draw();
		//HPバーの描画
		hpBar_->Draw();
	}

	if (animationFlag_ == true)
	{
		//死亡演出の描画
		deathParticleEmitter_->Draw();
	}
}

void Player::Reset()
{
	playerTrans_.translation = { 0.0f,-reticle_->GetReticleLimit() / 3,10.0f };
	hp_ = 10;
	reticle_->Reset();
}

void Player::HpSub()
{
	hp_--;
	hp_ = max(hp_, 0);
	damageFlag_ = true;
}

const bool Player::GetDamageFlag()
{
	return damageFlag_;
}

const Transform& Player::GetTransform()
{
	return playerTrans_;
}

const uint8_t Player::GetHp()
{
	return hp_;
}

void Player::SetDamageFlag(const bool damageFlag)
{
	damageFlag_ = damageFlag;
}

const myMath::Vector3& Player::GetAddTargetPos()
{
	return targetPos;
}

const bool& Player::GetDeathFlag()
{
	return deathFlag_;
}

void Player::Move()
{
	//先に補間先の座標を定義する
	float reticleX = reticle_->GetTransform().translation.x / 2;
	float reticleY = reticle_->GetTransform().translation.y / 2;
	//そのまま移動させると動きが硬いので補完する
	PhysicsMath::Complement(playerTrans_.translation.x, reticleX, 30.0f);
	PhysicsMath::Complement(playerTrans_.translation.y, reticleY, 30.0f);
}

void Player::Rotation(Camera* camera)
{
	//レティクルの方向に向くように回転
	playerTrans_.rotation.x = -std::atan2(directionVector_.y, directionVector_.z);
	playerTrans_.rotation.y = -std::atan2(directionVector_.z, directionVector_.x) + myMath::AX_PIF / 2;

	float angleZ = -(reticle_->GetTransform().translation.x / 2 - playerTrans_.translation.x) / 5.0f;
	//モデルのZ軸回転
	PhysicsMath::Complement(playerTrans_.rotation.z, angleZ, 15.0f);

	myMath::Vector3 cameraFrontVec = camera->GetTarget() - camera->GetEye();
	myMath::Vector3 cameraUp =
	{
		sinf(cameraFrontVec.y)* sinf(playerTrans_.rotation.z),
		cosf(cameraFrontVec.y) * cosf(playerTrans_.rotation.z),
		0.0f
	};
	camera->SetUp(cameraUp);

	//プレイヤーの横向きの回転をワールド座標に変換し、後でカメラに足せるように変数に格納
	targetPos = (playerTrans_.matWorld.Transform(playerTrans_.matWorld, { 0,0,1 }) - playerTrans_.matWorld.Transform(playerTrans_.matWorld, { 0,0,0 })) * 0.1f;

	ImGui::Begin("rot");
	ImGui::InputFloat3("playerRot", &playerTrans_.rotation.x);
	ImGui::InputFloat3("cameraFrontVec", &cameraFrontVec.x);
	myMath::Vector3 cameraTarget = camera->GetTarget();
	ImGui::InputFloat3("cameraTarget", &cameraTarget.x);
	myMath::Vector3 cameraPos = camera->GetEye();
	ImGui::InputFloat3("cameraPos", &cameraPos.x);
	ImGui::End();
}

void Player::BulletUpdate(Camera* camera)
{
	if (hp_ > 0)
	{
		if (input_->KeyboardTriggerPush(DIK_SPACE) || input_->ControllerButtonTriggerPush(A))
		{
			//弾の作成
			CreateBullet(playerTrans_.parentToTranslation, parentToDirectionVector_, BulletOwner::Player);
		}
	}

	//弾の更新処理
	Character::BulletUpdate(camera);
}

void Player::BulletDraw()
{
	for (const std::unique_ptr<Bullet>& bullet : bullets_)
	{
		//弾の描画
		bullet->Draw();
	}
}

void Player::SmokeUpdate(Camera* camera)
{
	//エンジンの座標に合わせるため、モデルの中心座標から位置をずらせるように子を作成
	smokeTrans_.parent = &playerTrans_;
	//モデルの中心座標から位置をずらす
	smokeTrans_.translation = { 0.0f,-0.3f,-4.0f };
	//子の更新処理
	smokeTrans_.TransUpdate(camera);
	//パーティクルを毎フレーム作成
	smokeEmitter_->Create(smokeTrans_.parentToTranslation);
	//パーティクルの更新
	smokeEmitter_->Update(camera);
}