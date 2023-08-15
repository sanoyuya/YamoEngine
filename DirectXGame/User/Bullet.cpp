#include "Bullet.h"
#include"ColliderManager.h"

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

void Bullet::Initialize()
{
	bullet_ = std::make_unique<Model>();
	bulletTex_ = bullet_->CreateObjModel("Resources/sphere");
	bullet_->SetModel(bulletTex_);
	bulletTrans_.Initialize();

	//パーティクルの初期化
	smokeEmitter_ = std::make_unique<PlayerEngineSmokeParticleEmitter>();
	smokeEmitter_->Initialize();
	smokeTrans_.Initialize();

	collisionData_.radius = 1.0f;
}

void Bullet::Update()
{
	deathTimer_++;
	if (deathTimer_ > maxDeathTime_)
	{
		isDead_ = true;
		ColliderManager::GetInstance()->SubCollision(this);
	}
	bulletTrans_.translation += directionVector_ * speed_;
	bulletTrans_.TransUpdate(camera_);

	collisionData_.center = bulletTrans_.translation;

	SmokeUpdate();
}

void Bullet::Draw()
{
	bullet_->DrawModel(&bulletTrans_);
	smokeEmitter_->Draw();
}

std::string Bullet::GetName()
{
	return name_;
}

const CollisionData& Bullet::GetCollisionData()
{
	return collisionData_;
}

void Bullet::SetCamera(Camera* camera)
{
	camera_ = camera;
}

void Bullet::SetPos(const myMath::Vector3& position)
{
	bulletTrans_.translation = position;
}

void Bullet::SetDirectionVector(const myMath::Vector3& directionVector)
{
	directionVector_ = directionVector;
}

void Bullet::SetOwner(BulletOwner owner)
{
	owner_ = owner;
}

void Bullet::SetName(const std::string& name)
{
	name_ = name;
}

void Bullet::SmokeUpdate()
{
	//エンジンの座標に合わせるため、モデルの中心座標から位置をずらせるように子を作成
	smokeTrans_.parent = &bulletTrans_;
	//モデルの中心座標から位置をずらす
	smokeTrans_.translation = { 0.0f,0.0f,-1.0f };
	//子の更新処理
	smokeTrans_.TransUpdate(camera_);
	//パーティクルを毎フレーム作成
	smokeEmitter_->Create(smokeTrans_.parentToTranslation);
	//パーティクルの更新
	smokeEmitter_->Update(camera_);
}

bool Bullet::GetIsDead()
{
	return isDead_;
}

void Bullet::OnCollision()
{
	isDead_ = true;
	ColliderManager::GetInstance()->SubCollision(this);
}

const myMath::Vector3& Bullet::GetPosition()
{
	return bulletTrans_.translation;
}