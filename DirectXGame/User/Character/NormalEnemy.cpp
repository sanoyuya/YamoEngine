#include "NormalEnemy.h"
#include"ColliderManager.h"

NormalEnemy::~NormalEnemy()
{
	if (deathAnimationFlag_ == false)
	{
		ColliderManager::GetInstance()->SubCollision(this);
	}
}

void NormalEnemy::Initialize()
{
	enemy_ = std::make_unique<Model>();
	enemyTex_ = enemy_->CreateObjModel("Resources/enemy");
	enemy_->SetModel(enemyTex_);
	enemyTrans_.Initialize();
	enemyTrans_.scale = { 10.0f,10.0f,10.0f };

	// パーティクル生成
	emitter_ = std::make_unique<EnemyDeathParticleEmitter>();
	emitter_->Initialize();
}

void NormalEnemy::Update()
{
	//出現していたら
	if (spawnFlag_ == true)
	{
		//敵のモデルの更新処理
		enemyTrans_.TransUpdate(camera_);
		//弾の生成処理と更新処理
		BulletUpdate(camera_);
		//死亡処理
		DeathUpdate(camera_, gameTimer_);
	}
	else
	{
		SpawnUpdate(camera_, gameTimer_);
	}

	collisionData_.center = enemyTrans_.translation;
}

void NormalEnemy::Draw()
{
	//死んでいないときのみ描画
	if (spawnFlag_ == true && deathAnimationFlag_ == false)
	{
		enemy_->DrawModel(&enemyTrans_);
	}
	else
	{
		//死亡演出の描画処理
		emitter_->Draw();
	}

	//弾の描画処理
	BulletDraw();
}

std::string NormalEnemy::GetName()
{
	return name_;
}

const CollisionData& NormalEnemy::GetCollisionData()
{
	return collisionData_;
}

void NormalEnemy::SetPosition(const myMath::Vector3& position)
{
	enemyTrans_.translation = position;
}

void NormalEnemy::SetRotation(const myMath::Vector3& rotation)
{
	enemyTrans_.rotation = rotation;
}

void NormalEnemy::SetColliderSize(const float size)
{
	collisionData_.radius = size;
}

void NormalEnemy::SetSpawnTimer(const float timer)
{
	spawnTime_ = timer;
}

void NormalEnemy::SetDeathTimer(const float timer)
{
	deathTime_ = timer;
}

const bool NormalEnemy::GetIsDead()
{
	return isDead_;
}

bool NormalEnemy::GetSpawnFlag()
{
	return spawnFlag_;
}

const bool NormalEnemy::GetDeathAnimationFlag()
{
	return deathAnimationFlag_;
}

void NormalEnemy::OnCollision()
{
	emitter_->Create(enemyTrans_.parentToTranslation);
	deathAnimationFlag_ = true;
}

bool NormalEnemy::GetLockOnFlag()
{
	return lockOnFlag;
}

const Transform& NormalEnemy::GetTrans()
{
	return enemyTrans_;
}

void NormalEnemy::BulletUpdate(Camera* camera)
{
	myMath::Vector3 frontVec = player_->GetTransform().parentToTranslation - enemyTrans_.translation;
	frontVec = frontVec.normalization();

	if (deathAnimationFlag_ == false)
	{
		float length = sqrt((player_->GetTransform().parentToTranslation.x - enemyTrans_.parentToTranslation.x) * (player_->GetTransform().parentToTranslation.x - enemyTrans_.parentToTranslation.x)) +
			sqrt((player_->GetTransform().parentToTranslation.z - enemyTrans_.parentToTranslation.z) * (player_->GetTransform().parentToTranslation.z - enemyTrans_.parentToTranslation.z));
		bulletTimer++;
		if (bulletTimer > maxBulletTime)
		{
			if (75.0f >= length)
			{
				CreateBullet(enemyTrans_.translation, frontVec, BulletOwner::Enemy);
			}

			bulletTimer = 0.0f;
		}
	}

	//弾の更新処理
	BulletManager::BulletUpdate(camera);
}

void NormalEnemy::BulletDraw()
{
	for (const std::unique_ptr<Bullet>& bullet : bullets_)
	{
		bullet->Draw();//弾の描画
	}
}

void NormalEnemy::SpawnUpdate(Camera* camera, GameTimer* gameTimer)
{
	if (spawnTime_ <= gameTimer->GetIntTime())
	{
		enemyTrans_.TransUpdate(camera);
		if (spawnAnimationFlag_ == false)
		{
			emitter_->Create(enemyTrans_.parentToTranslation);
		}
		spawnAnimationFlag_ = true;
	}

	if (spawnAnimationFlag_ == true)
	{
		emitter_->Update(camera);
		spawnAnimationTimer_++;
	}

	if (spawnAnimationTimer_ > maxSpawnAnimationTime_)
	{
		spawnFlag_ = true;
	}
}

void NormalEnemy::DeathUpdate(Camera* camera, GameTimer* gameTimer)
{
	//死亡時間になったら死ぬ
	if (deathTime_ <= gameTimer->GetIntTime())
	{
		isDead_ = true;
	}

	//死亡演出の更新処理
	if (deathAnimationFlag_ == true)
	{
		emitter_->Update(camera);
		deathAnimationTimer_++;
	}

	if (deathAnimationTimer_ > maxDeathAnimationTime_)
	{
		isDead_ = true;
	}
}