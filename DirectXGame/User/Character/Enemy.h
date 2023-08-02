#pragma once
#include"DrawOversight.h"
#include<list>
#include"Player.h"
#include"EnemyDeathParticleEmitter.h"
#include"GameTimer.h"

class Enemy :public Character
{
private:

	std::unique_ptr<Model>enemy_;
	uint32_t enemyTex_ = 0;
	Transform enemyTrans_;
	float colliderSize_ = 0.0f;
	float spawnTime_ = 0.0f;
	float deathTime_ = 0.0f;
	bool isDead_ = false;

	float bulletTimer = 0.0f;
	const float maxBulletTime = 60.0f;

	bool deathAnimationFlag_ = false;
	uint8_t deathAnimationTimer_ = 0;
	const uint8_t maxDeathAnimationTime_ = 60;

	std::unique_ptr<EnemyDeathParticleEmitter>emitter_;

public:

	Enemy();
	~Enemy();

	void Initialize();

	void Update(Camera* camera, Player*player, GameTimer* gameTimer);

	void Draw();

public://セッター

	//blenderで出力したデータを読み込むときに初期座標をセットする関数
	void SetPosition(const myMath::Vector3& position);
	//blenderで出力したデータを読み込むときに初期角度をセットする関数
	void SetRotation(const myMath::Vector3& rotation);
	//blenderで出力したデータを読み込むときに当たり判定の大きさをセットする関数
	void SetColliderSize(const float size);
	//blenderで出力したデータを読み込むときにスポーンタイマーをセットする関数
	void SetSpawnTimer(const float timer);
	//blenderで出力したデータを読み込むときに死亡タイマーをセットする関数
	void SetDeathTimer(const float timer);

public://ゲッター

	const Transform& GetTrans();

	const float& GetColliderSize();

	bool GetIsDead();

	bool GetDeathAnimationFlag();

	void OnCollision();

private:

	void BulletUpdate(Camera* camera, Player* player);

	void BulletDraw();

	void DeathUpdate(Camera* camera, GameTimer* gameTimer);
};