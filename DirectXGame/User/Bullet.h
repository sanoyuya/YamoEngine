#pragma once
#include"DrawOversight.h"

enum class BulletOwner
{
	Player,
	Enemy
};

class Bullet
{
private:

	bool isDead_ = false;
	uint8_t deathTimer_ = 0;
	const uint8_t maxDeathTime_ = 60;

	myMath::Vector3 directionVector_;//方向ベクトル
	const float speed_ = 2.0f;

	std::unique_ptr<Model>bullet_;
	uint32_t bulletTex_ = 0;
	Transform bulletTrans_;

	BulletOwner owner_ = BulletOwner::Player;

public:

	Bullet();
	~Bullet();

	void Initialize(const myMath::Vector3& position,const myMath::Vector3& directionVector, BulletOwner owner);

	void Update(Camera* camera);

	void Draw();

public:

	bool GetIsDead();

	void OnCollision();

	const myMath::Vector3& GetPosition();
};