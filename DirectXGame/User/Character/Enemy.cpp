#include "Enemy.h"
#include"ColliderManager.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	enemy_ = std::make_unique<Model>();
	enemyTex_ = enemy_->CreateObjModel("Resources/F-35E");
	enemy_->SetModel(enemyTex_);
	enemyTrans_.Initialize();
}

void Enemy::Update(Camera* camera, Player* player)
{
	enemyTrans_.TransUpdate(camera);

	BulletUpdate(camera, player);
}

void Enemy::Draw()
{
	enemy_->DrawModel(&enemyTrans_);

	BulletDraw();
}

void Enemy::SetPosition(const myMath::Vector3& position)
{
	enemyTrans_.translation = position;
}

bool Enemy::GetIsDead()
{
	return isDead_;
}

void Enemy::BulletUpdate(Camera* camera, Player* player)
{
	myMath::Vector3 frontVec = player->GetTransform().parentToTranslation - enemyTrans_.translation;
	frontVec = frontVec.normalization();

	bullets_.remove_if([](std::unique_ptr<Bullet>& bullet) { return bullet->GetIsDead(); });

	bulletTimer++;
	if (bulletTimer >maxBulletTime)
	{
		//弾を生成し、初期化
		std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
		newBullet->Initialize(enemyTrans_.translation, frontVec);
		//弾を登録する
		bullets_.push_back(std::move(newBullet));

		bulletTimer = 0.0f;
	}

	//弾の更新処理
	for (const std::unique_ptr<Bullet>& bullet : bullets_)
	{
		bullet->Update(camera);
	}

	ColliderManager::GetInstance()->SetEnemyBulletList(bullets_);//弾リストをColliderManagerに送る
}

void Enemy::BulletDraw()
{
	for (const std::unique_ptr<Bullet>& bullet : bullets_)
	{
		bullet->Draw();//弾の描画
	}
}