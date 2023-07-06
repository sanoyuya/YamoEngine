#include "ColliderManager.h"

void ColliderManager::Update(Player* player)
{
	EnemyBulletToPlayer(player);
	PlayerBulletToEnemy();
}

void ColliderManager::AddPlayerBulletCollider(Bullet* collider)
{
	playersBulletsCollider_.push_back(collider);
}

void ColliderManager::AddEnemyCollider(Enemy* collider)
{
	enemysCollider_.push_back(collider);
}

void ColliderManager::AddEnemyBulletCollider(Bullet* collider)
{
	enemysBulletsCollider_.push_back(collider);
}

void ColliderManager::PlayerBulletToEnemy()
{
	for (auto& bullet : playersBulletsCollider_)
	{
		for (auto& enemys : enemysCollider_)
		{
			if (Collision::SphereToSphere(bullet->GetPosition(),1.0f, enemys->GetPosition(),2.0f))
			{
				bullet->OnCollision();
				enemys->OnCollision();
			}
		}
	}
}

void ColliderManager::EnemyBulletToPlayer(Player* player)
{
	for (auto& bullet : enemysBulletsCollider_)
	{
		if (Collision::SphereToSphere(player->GetTransform().translation, 2.0f, bullet->GetPosition(), 1.0f))
		{
			player->HpSub();
			bullet->OnCollision();
		}
	}
}

void ColliderManager::EnemyToPlayer(Player* player)
{
	for (auto& enemys : enemysCollider_)
	{
		if (Collision::SphereToSphere(player->GetTransform().translation, 2.0f, enemys->GetPosition(), 2.0f))
		{
			player->HpSub();
			enemys->OnCollision();
		}
	}
}

ColliderManager* ColliderManager::GetInstance()
{
	static ColliderManager instance;
	return &instance;
}