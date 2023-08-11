#include "MoveEnemy.h"
#include"PhysicsMath.h"

void MoveEnemy::Initialize()
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

void MoveEnemy::Update()
{
	time_++;
	addY = PhysicsMath::SimpleHarmonicMotion(time_,0.5f,120.0f);
	enemyTrans_.translation.y = enemyTrans_.translation.y+addY;

	//出現していたら
	if (spawnFlag_ == true)
	{
		//敵のモデルの更新処理
		enemyTrans_.TransUpdate(camera_);
		//弾の生成処理と更新処理
		BulletUpdate(camera_, player_);
		//死亡処理
		DeathUpdate(camera_, gameTimer_);
	}
	else
	{
		SpawnUpdate(camera_, gameTimer_);
	}
}

void MoveEnemy::Draw()
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

std::string MoveEnemy::GetName()
{
	return name_;
}

void MoveEnemy::SetPosition(const myMath::Vector3& position)
{
	enemyTrans_.translation = position;
}

void MoveEnemy::SetRotation(const myMath::Vector3& rotation)
{
	enemyTrans_.rotation = rotation;
}

void MoveEnemy::SetColliderSize(const float size)
{
	colliderSize_ = size;
}

void MoveEnemy::SetSpawnTimer(const float timer)
{
	spawnTime_ = timer;
}

void MoveEnemy::SetDeathTimer(const float timer)
{
	deathTime_ = timer;
}

const Transform& MoveEnemy::GetTrans()
{
	return enemyTrans_;
}

const float& MoveEnemy::GetColliderSize()
{
	return colliderSize_;
}

bool MoveEnemy::GetIsDead()
{
	return isDead_;
}

bool MoveEnemy::GetSpawnFlag()
{
	return spawnFlag_;
}

bool MoveEnemy::GetDeathAnimationFlag()
{
	return deathAnimationFlag_;
}

void MoveEnemy::OnCollision()
{
	deathAnimationFlag_ = true;
	emitter_->Create(enemyTrans_.parentToTranslation);
}

bool MoveEnemy::GetLockOnFlag()
{
	return lockOnFlag;
}

void MoveEnemy::BulletUpdate(Camera* camera, Player* player)
{
	myMath::Vector3 frontVec = player->GetTransform().parentToTranslation - enemyTrans_.translation;
	frontVec = frontVec.normalization();

	if (deathAnimationFlag_ == false)
	{
		float length = sqrt((player->GetTransform().parentToTranslation.x - enemyTrans_.parentToTranslation.x) * (player->GetTransform().parentToTranslation.x - enemyTrans_.parentToTranslation.x)) +
			sqrt((player->GetTransform().parentToTranslation.z - enemyTrans_.parentToTranslation.z) * (player->GetTransform().parentToTranslation.z - enemyTrans_.parentToTranslation.z));
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

void MoveEnemy::BulletDraw()
{
	for (const std::unique_ptr<Bullet>& bullet : bullets_)
	{
		bullet->Draw();//弾の描画
	}
}

void MoveEnemy::SpawnUpdate(Camera* camera, GameTimer* gameTimer)
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

void MoveEnemy::DeathUpdate(Camera* camera, GameTimer* gameTimer)
{
	//死亡時間になったら死ぬ
	if (deathTime_ <= gameTimer->GetIntTime())
	{
		if (deathAnimationFlag_ == false)
		{
			emitter_->Create(enemyTrans_.parentToTranslation);
		}
		deathAnimationFlag_ = true;
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