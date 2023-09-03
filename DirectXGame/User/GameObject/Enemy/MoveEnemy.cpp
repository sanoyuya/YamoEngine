#include "MoveEnemy.h"
#include"PhysicsMath.h"
#include"ColliderManager.h"
#include"EasingFunction.h"

MoveEnemy::~MoveEnemy()
{
	if (deathAnimationFlag_ == false)
	{
		ColliderManager::GetInstance()->SubCollision(this);
	}
	if (lockOnFlag == true)
	{
		ColliderManager::GetInstance()->SubLockOnEnemy(this);
	}
}

void MoveEnemy::Initialize()
{
	enemy_ = std::make_unique<Model>();
	enemyTex_ = enemy_->CreateObjModel("Resources/enemy");
	enemy_->SetModel(enemyTex_);
	enemyTrans_.Initialize();
	enemyTrans_.translation = moveEnemyProperty_.spawnPos;
	enemyTrans_.rotation = { myMath::ChangeRadians(moveEnemyProperty_.movePosRotation.x), myMath::ChangeRadians(moveEnemyProperty_.movePosRotation.y) - myMath::AX_PIF / 2, myMath::ChangeRadians(moveEnemyProperty_.movePosRotation.z) - myMath::AX_PIF / 2 };
	enemyTrans_.scale = { 10.0f,10.0f,10.0f };

	//���S�A�j���[�V�����p�[�e�B�N��������
	emitter_ = std::make_unique<EnemyDeathParticleEmitter>();
	emitter_->Initialize();
	//�X�|�[���A�j���[�V�����p�[�e�B�N��������
	spawnEmitter_= std::make_unique<EnemySpawnParticleEmitter>();
	spawnEmitter_->Initialize();

	lockOnAnimation_ = std::make_unique<LockOnAnimation>();
	lockOnAnimation_->Initialize();
}

void MoveEnemy::Update()
{
	//�o�����Ă�����
	if (spawnFlag_ == true)
	{
		PhaseUpdate();
		//�G�̃��f���̍X�V����
		enemyTrans_.TransUpdate(camera_);
		collisionData_.center = enemyTrans_.translation;
		lockOnAnimation_->Update(enemyTrans_.parentToTranslation, camera_);
		//�e�̐��������ƍX�V����
		BulletUpdate();
		//���S����
		DeathUpdate();
	}
	else
	{
		SpawnUpdate();
	}
}

void MoveEnemy::PhaseUpdate()
{
	switch (phase)
	{
	case ActionPhase::MOVE:

		PhaseMove(moveEnemyProperty_.spawnPos, moveEnemyProperty_.movePos, moveEnemyProperty_.spawnPosRotation, moveEnemyProperty_.movePosRotation, moveEnemyProperty_.toMovePosTime * 60);

		actionTimer++;

		if (moveEnemyProperty_.toMovePosTime * 60 <= actionTimer)
		{
			actionTimer = 0;
			phase = ActionPhase::WAIT;
		}

		break;
	case ActionPhase::WAIT:

		if (moveEnemyProperty_.waitTime * 60 <= actionTimer)
		{
			actionTimer = 0;
			waitFinishPos = enemyTrans_.translation;
			waitFinishRot = enemyTrans_.rotation;
			phase = ActionPhase::ESCAPE;
		}

		addY = PhysicsMath::SimpleHarmonicMotion(actionTimer, 0.02f, 120.0f);
		enemyTrans_.translation.y = enemyTrans_.translation.y + addY;

		actionTimer++;

		break;
	case ActionPhase::ESCAPE:

		PhaseMove(waitFinishPos, moveEnemyProperty_.escapePos, waitFinishRot, moveEnemyProperty_.escapePosRotation, moveEnemyProperty_.toEscapePosTime * 60);

		actionTimer++;

		if (moveEnemyProperty_.toEscapePosTime * 60 <= actionTimer)
		{
			isDead_ = true;
		}

		break;
	}
}

void MoveEnemy::PhaseMove(const myMath::Vector3& startPosition, const myMath::Vector3& endPosition, const myMath::Vector3& startRotation, const myMath::Vector3& endRotation, const float maxTime)
{
	enemyTrans_.translation = { static_cast<float>(Easing::EaseInOutCubic(actionTimer, startPosition.x, endPosition.x, maxTime)),
	static_cast<float>(Easing::EaseInOutCubic(actionTimer, startPosition.y, endPosition.y, maxTime)),
	static_cast<float>(Easing::EaseInOutCubic(actionTimer, startPosition.z, endPosition.z, maxTime)) };

	enemyTrans_.rotation = { static_cast<float>(Easing::EaseInOutCubic(actionTimer, startRotation.x, endRotation.x, maxTime)),
	static_cast<float>(Easing::EaseInOutCubic(actionTimer, startRotation.y, endRotation.y, maxTime)),
	static_cast<float>(Easing::EaseInOutCubic(actionTimer, startRotation.z, endRotation.z, maxTime)) };
}

void MoveEnemy::Draw()
{
	if (spawnFlag_ == false)
	{
		spawnEmitter_->Draw();
	}
	//����ł��Ȃ��Ƃ��̂ݕ`��
	if (spawnFlag_ == true && deathAnimationFlag_ == false)
	{
		enemy_->DrawModel(&enemyTrans_);
		if (lockOnFlag == true)
		{
			lockOnAnimation_->Draw(camera_);
		}
	}
	else
	{
		//���S���o�̕`�揈��
		emitter_->Draw();
	}
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

void MoveEnemy::SetMoveEnemyProperty(const MoveEnemyProperty& moveEnemyProperty)
{
	moveEnemyProperty_ = moveEnemyProperty;
	moveEnemyProperty_.spawnPosRotation = { myMath::ChangeRadians(moveEnemyProperty_.spawnPosRotation.x), myMath::ChangeRadians(moveEnemyProperty_.spawnPosRotation.y) - myMath::AX_PIF / 2, myMath::ChangeRadians(moveEnemyProperty_.spawnPosRotation.z) - myMath::AX_PIF / 2 };
	moveEnemyProperty_.movePosRotation = { myMath::ChangeRadians(moveEnemyProperty_.movePosRotation.x), myMath::ChangeRadians(moveEnemyProperty_.movePosRotation.y) - myMath::AX_PIF / 2, myMath::ChangeRadians(moveEnemyProperty_.movePosRotation.z) - myMath::AX_PIF / 2 };
	moveEnemyProperty_.escapePosRotation = { myMath::ChangeRadians(moveEnemyProperty_.escapePosRotation.x), myMath::ChangeRadians(moveEnemyProperty_.escapePosRotation.y) - myMath::AX_PIF / 2, myMath::ChangeRadians(moveEnemyProperty_.escapePosRotation.z) - myMath::AX_PIF / 2 };
}

const bool MoveEnemy::GetIsDead()
{
	return isDead_;
}

bool MoveEnemy::GetSpawnFlag()
{
	return spawnFlag_;
}

const bool MoveEnemy::GetDeathAnimationFlag()
{
	return deathAnimationFlag_;
}

void MoveEnemy::LockOn()
{
	lockOnFlag = true;
	lockOnAnimation_->Create();
}

void MoveEnemy::CancelLockOn()
{
	lockOnFlag = false;
	lockOnAnimation_->Cancel();
}

void MoveEnemy::OnCollision()
{
	emitter_->Create(enemyTrans_.parentToTranslation);
	deathAnimationFlag_ = true;
}

const bool MoveEnemy::GetLockOnFlag()
{
	return lockOnFlag;
}

const Transform& MoveEnemy::GetTrans()
{
	return enemyTrans_;
}

void MoveEnemy::BulletUpdate()
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
				bulletManager_->CreateNormalBullet(enemyTrans_.translation, frontVec, BulletOwner::Enemy);
			}

			bulletTimer = 0.0f;
		}
	}
}

void MoveEnemy::SpawnUpdate()
{
	if (spawnTime_ <= gameTimer_->GetIntTime())
	{
		enemyTrans_.TransUpdate(camera_);

		if (spawnAnimationTimer_ < maxSpawnAnimationTime_ / 2)
		{
			spawnEmitter_->Create(enemyTrans_.parentToTranslation);
		}
		spawnAnimationTimer_++;
	}

	spawnEmitter_->Update(camera_);

	if (spawnAnimationTimer_ > maxSpawnAnimationTime_)
	{
		spawnFlag_ = true;
	}
}

void MoveEnemy::DeathUpdate()
{
	//���S���ԂɂȂ����玀��
	if (deathTime_ <= gameTimer_->GetIntTime())
	{
		isDead_ = true;
	}

	//���S���o�̍X�V����
	if (deathAnimationFlag_ == true)
	{
		emitter_->Update(camera_);
		deathAnimationTimer_++;
	}

	if (deathAnimationTimer_ > maxDeathAnimationTime_)
	{
		isDead_ = true;
	}
}