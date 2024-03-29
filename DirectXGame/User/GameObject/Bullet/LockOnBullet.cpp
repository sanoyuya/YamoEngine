#include "LockOnBullet.h"
#include"ColliderManager.h"
#include"EasingFunction.h"
#include"PhysicsMath.h"

LockOnBullet::~LockOnBullet()
{
	ColliderManager::GetInstance()->SubCollision(this);
}

void LockOnBullet::Initialize()
{
	bullet_ = std::make_unique<Model>();
	bulletTex_ = bullet_->CreateObjModel("Resources/sphere");
	bullet_->SetModel(bulletTex_);
	bulletTrans_.Initialize();

	//パーティクルの初期化
	smokeEmitter_->SetSize(scale_);

	actualTrans_.Initialize();
	isPlus_ = myMath::GetRandPlusOrMinus();

	collisionData_.radius = 1.0f;
}

void LockOnBullet::Update()
{
	deathTimer_++;
	if (deathTimer_ > maxDeathTime_)
	{
		isDead_ = true;
		ColliderManager::GetInstance()->CancelLockOn();
	}
	
	BulletMove();

	collisionData_.center = bulletTrans_.translation;

	//パーティクルを毎フレーム作成
	smokeEmitter_->SetColor(color_);
	smokeEmitter_->Create(bulletTrans_.parentToTranslation);
}

void LockOnBullet::Draw()
{
	//当たり判定用描画
	//bullet_->DrawModel(&bulletTrans_, color_);
}

std::string LockOnBullet::GetName()
{
	return name_;
}

const Transform& LockOnBullet::GetTransform()
{
	return bulletTrans_;
}

const CollisionData& LockOnBullet::GetCollisionData()
{
	return collisionData_;
}

void LockOnBullet::OnCollision()
{
	isDead_ = true;
}

const bool LockOnBullet::GetIsDead()
{
	return isDead_;
}

const bool LockOnBullet::GetDeathAnimationFlag()
{
	return false;
}

void LockOnBullet::LockOn()
{
}

void LockOnBullet::CancelLockOn()
{
}

const bool LockOnBullet::GetLockOnFlag()
{
	return false;
}

void LockOnBullet::SetPos(const myMath::Vector3& position)
{
	bulletTrans_.translation = position;//念のため
	startPos_ = position;
}

void LockOnBullet::SetDirectionVector(const myMath::Vector3& directionVector)
{
	directionVector;
}

void LockOnBullet::SetOwner(BulletOwner owner)
{
	owner_ = owner;
}

void LockOnBullet::SetName(const std::string& name)
{
	name_ = name;
}

void LockOnBullet::SetTargetPos(GameObject* lockOnEnemy)
{
	lockOnEnemy_ = lockOnEnemy;
}

void LockOnBullet::SetControlPos(const myMath::Vector3& position)
{
	controlPos_ = position;
}

void LockOnBullet::BulletMove()
{
	if (lockOnEnemy_!=nullptr)
	{
		endPos = lockOnEnemy_->GetTransform().translation;
	}
	//弾の移動にベジエ補間をかける
	bulletTrans_.translation = myMath::Beziers(startPos_, endPos, controlPos_, beziersTime_ / static_cast<float>(maxDeathTime_ / sGameTimer_->GetTimeSpeed()));

	//ベジエ補間にイージング補間をかける
	beziersTime_ = static_cast<float>(Easing::EaseInOutSine(deathTimer_, 0.0f, static_cast<float>(maxDeathTime_ / sGameTimer_->GetTimeSpeed()), static_cast<float>(maxDeathTime_ / sGameTimer_->GetTimeSpeed())));

	bulletTrans_.TransUpdate(sCamera_);
}