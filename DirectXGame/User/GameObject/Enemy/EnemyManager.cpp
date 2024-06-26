#include "EnemyManager.h"
#include"ColliderManager.h"
#include<imgui.h>

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::Initialize(EnemyList& enemyData, Player* player, BulletManager* bulletManager)
{
	//敵をjsonファイルから読み込む
	Load(enemyData);

	for (std::unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Initialize();
		ColliderManager::GetInstance()->AddCollision(enemy.get());
		enemy->SetPlayer(player);
		enemy->SetBulletManager(bulletManager);
	}
}

void EnemyManager::Update()
{
	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) { return enemy->GetIsDead(); });
	for (const std::unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Update();
	}
}

void EnemyManager::Draw()
{
	for (const std::unique_ptr<Enemy>& enemy : enemys_)
	{
		enemy->Draw();
	}
}

void EnemyManager::Load(EnemyList& enemyData)
{
	std::swap(enemys_, enemyData.enemys);
}

void EnemyManager::ReLoad(EnemyList& enemyData,Player* player, BulletManager* bulletManager)
{
	Reset();
	Initialize(enemyData, player, bulletManager);
}

void EnemyManager::Reset()
{
	enemys_.clear();
}

const std::list<std::unique_ptr<Enemy>>& EnemyManager::GetEnemyList()
{
	return enemys_;
}