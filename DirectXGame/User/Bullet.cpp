#include "Bullet.h"

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

void Bullet::Initialize(const myMath::Vector3& position, const myMath::Vector3& directionVector)
{
	bullet_ = std::make_unique<Model>();
	bulletTex_ = bullet_->CreateObjModel("Resources/sphere");
	bullet_->SetModel(bulletTex_);
	bulletTrans_.Initialize();
	bulletTrans_.translation = position;
	directionVector_ = directionVector;
}

void Bullet::Update(Camera* camera)
{
	deathTimer_++;
	if (deathTimer_ > maxDeathTime_)
	{
		isDead_ = true;
	}
	bulletTrans_.translation += directionVector_ * speed_;
	bulletTrans_.TransUpdate(camera);
}

void Bullet::Draw()
{
	bullet_->DrawModel(&bulletTrans_);
}

void Bullet::SetOwner(const BulletOwner& owner)
{
	owner_ = owner;
}

bool Bullet::GetIsDead()
{
	return isDead_;
}
