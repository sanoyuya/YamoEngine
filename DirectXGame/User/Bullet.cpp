#include "Bullet.h"
#include"ColliderManager.h"

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

void Bullet::Initialize(const myMath::Vector3& position, const myMath::Vector3& directionVector, BulletOwner owner)
{
	bullet_ = std::make_unique<Model>();
	bulletTex_ = bullet_->CreateObjModel("Resources/sphere");
	bullet_->SetModel(bulletTex_);
	bulletTrans_.Initialize();
	bulletTrans_.translation = position;
	directionVector_ = directionVector;

	owner_ = owner;
}

void Bullet::Update(Camera* camera)
{
	deathTimer_++;
	if (deathTimer_ > maxDeathTime_)
	{
		isDead_ = true;
		if (owner_ == BulletOwner::Player)
		{
			ColliderManager::GetInstance()->SubPlayerBullet(this);
		}
		else if (owner_ == BulletOwner::Enemy)
		{
			ColliderManager::GetInstance()->SubEnemyBullet(this);
		}
	}
	bulletTrans_.translation += directionVector_ * speed_;
	bulletTrans_.TransUpdate(camera);
}

void Bullet::Draw()
{
	bullet_->DrawModel(&bulletTrans_);
}

bool Bullet::GetIsDead()
{
	return isDead_;
}

void Bullet::OnCollision()
{
	isDead_ = true;
}

const myMath::Vector3& Bullet::GetPosition()
{
	return bulletTrans_.translation;
}