#include "MoveEnemy.h"
#include"PhysicsMath.h"
#include"ColliderManager.h"

MoveEnemy::~MoveEnemy()
{
	if (deathAnimationFlag_ == false)
	{
		ColliderManager::GetInstance()->SubCollision(this);
	}
}

void MoveEnemy::Initialize()
{
	enemy_ = std::make_unique<Model>();
	enemyTex_ = enemy_->CreateObjModel("Resources/enemy");
	enemy_->SetModel(enemyTex_);
	enemyTrans_.Initialize();
	enemyTrans_.scale = { 10.0f,10.0f,10.0f };

	// �p�[�e�B�N������
	emitter_ = std::make_unique<EnemyDeathParticleEmitter>();
	emitter_->Initialize();
}

void MoveEnemy::Update()
{
	time_++;
	addY = PhysicsMath::SimpleHarmonicMotion(time_,0.5f,120.0f);
	enemyTrans_.translation.y = enemyTrans_.translation.y + addY;

	collisionData_.center = enemyTrans_.translation;

	//�o�����Ă�����
	if (spawnFlag_ == true)
	{
		//�G�̃��f���̍X�V����
		enemyTrans_.TransUpdate(camera_);
		//�e�̐��������ƍX�V����
		BulletUpdate(camera_);
		//���S����
		DeathUpdate(camera_, gameTimer_);
	}
	else
	{
		SpawnUpdate(camera_, gameTimer_);
	}
}

void MoveEnemy::Draw()
{
	//����ł��Ȃ��Ƃ��̂ݕ`��
	if (spawnFlag_ == true && deathAnimationFlag_ == false)
	{
		enemy_->DrawModel(&enemyTrans_);
	}
	else
	{
		//���S���o�̕`�揈��
		emitter_->Draw();
	}

	//�e�̕`�揈��
	BulletDraw();
}

std::string MoveEnemy::GetName()
{
	return name_;
}

const CollisionData& MoveEnemy::GetCollisionData()
{
	return collisionData_;
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
	collisionData_.radius = size;
}

void MoveEnemy::SetSpawnTimer(const float timer)
{
	spawnTime_ = timer;
}

void MoveEnemy::SetDeathTimer(const float timer)
{
	deathTime_ = timer;
}

const bool MoveEnemy::GetIsDead()
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
	if (deathAnimationFlag_ == false)
	{
		emitter_->Create(enemyTrans_.parentToTranslation);
	}
	deathAnimationFlag_ = true;
}

bool MoveEnemy::GetLockOnFlag()
{
	return lockOnFlag;
}

const Transform& MoveEnemy::GetTrans()
{
	return enemyTrans_;
}

void MoveEnemy::BulletUpdate(Camera* camera)
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

	//�e�̍X�V����
	BulletManager::BulletUpdate(camera);
}

void MoveEnemy::BulletDraw()
{
	for (const std::unique_ptr<Bullet>& bullet : bullets_)
	{
		bullet->Draw();//�e�̕`��
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
	//���S���ԂɂȂ����玀��
	if (deathTime_ <= gameTimer->GetIntTime())
	{
		isDead_ = true;
	}

	//���S���o�̍X�V����
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