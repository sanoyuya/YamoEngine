#pragma once
#include"BulletBase.h"
#include"DrawOversight.h"
#include"PlayerEngineSmokeParticleEmitter.h"

class LockOnBullet :public BulletBase
{
private:

	std::string name_ = "lockOnBullet";

	bool isDead_ = false;
	uint8_t deathTimer_ = 0;
	const uint8_t maxDeathTime_ = 80;

	myMath::Vector3 directionVector_;//方向ベクトル

	std::unique_ptr<Model>bullet_;
	uint32_t bulletTex_ = 0;
	Transform bulletTrans_;

	BulletOwner owner_ = BulletOwner::Player;

	Transform smokeTrans_;
	Transform actualTrans_;
	float angle_ = 0.0f;
	float isPlus_ = 0.0f;

	CollisionData collisionData_;

	myMath::Vector3 startPos_ = {};
	myMath::Vector3 targetPos_ = {};
	myMath::Vector3 controlPos_ = {};

	float beziersTime_ = 0.0f;

	GameObject* lockOnEnemy_ = nullptr;

	myMath::Vector3 endPos = {};

public://BulletBaseで必要な奴

	//デストラクタ
	~LockOnBullet();

	//初期化処理
	void Initialize()override;

	//更新処理
	void Update()override;

	//描画処理
	void Draw()override;

	//オブジェクトの名前取得処理
	std::string GetName()override;

	const Transform& GetTransform()override;

	//オブジェクトの当たり判定取得処理
	const CollisionData& GetCollisionData()override;

	//オブジェクトの衝突判定処理
	void OnCollision()override;

	//死亡フラグの取得
	const bool GetIsDead()override;

	//死亡アニメーションフラグの取得
	const bool GetDeathAnimationFlag()override;

	//ロックオン
	void LockOn()override;

	//ロックオン解除
	void CancelLockOn()override;

	const bool GetLockOnFlag()override;

	void SetPos(const myMath::Vector3& position)override;
	void SetDirectionVector(const myMath::Vector3& directionVector)override;
	void SetOwner(BulletOwner owner)override;
	void SetName(const std::string& name)override;
	void SetTargetPos(GameObject* lockOnEnemy)override;
	void SetControlPos(const myMath::Vector3& position)override;

private:

	void BulletMove();
};