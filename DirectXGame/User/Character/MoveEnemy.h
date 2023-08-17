#pragma once
#include"Enemy.h"

class MoveEnemy :public Enemy, public BulletManager
{
private:

	std::string name_ = "enemy";

	std::unique_ptr<Model>enemy_;
	uint32_t enemyTex_ = 0;
	Transform enemyTrans_;
	float spawnTime_ = 0.0f;
	float deathTime_ = 0.0f;

	bool spawnFlag_ = false;
	bool isDead_ = false;

	float bulletTimer = 0.0f;
	const float maxBulletTime = 60.0f;

	bool spawnAnimationFlag_ = false;
	uint8_t spawnAnimationTimer_ = 0;
	const uint8_t maxSpawnAnimationTime_ = 60;

	bool deathAnimationFlag_ = false;
	uint8_t deathAnimationTimer_ = 0;
	const uint8_t maxDeathAnimationTime_ = 60;

	std::unique_ptr<EnemyDeathParticleEmitter>emitter_;

	bool lockOnFlag = false;

	float time_ = 0.0f;
	float addY = 0.0f;

	CollisionData collisionData_;

public:

	MoveEnemy() = default;
	~MoveEnemy();

	void Initialize();

	void Update();

	void Draw();

	std::string GetName();

	//�I�u�W�F�N�g�̓����蔻��擾����
	const CollisionData& GetCollisionData();

	//�I�u�W�F�N�g�̏Փ˔��菈��
	void OnCollision();

public://�Z�b�^�[

	//blender�ŏo�͂����f�[�^��ǂݍ��ނƂ��ɏ������W���Z�b�g����֐�
	void SetPosition(const myMath::Vector3& position);
	//blender�ŏo�͂����f�[�^��ǂݍ��ނƂ��ɏ����p�x���Z�b�g����֐�
	void SetRotation(const myMath::Vector3& rotation);
	//blender�ŏo�͂����f�[�^��ǂݍ��ނƂ��ɓ����蔻��̑傫�����Z�b�g����֐�
	void SetColliderSize(const float size);
	//blender�ŏo�͂����f�[�^��ǂݍ��ނƂ��ɃX�|�[���^�C�}�[���Z�b�g����֐�
	void SetSpawnTimer(const float timer);
	//blender�ŏo�͂����f�[�^��ǂݍ��ނƂ��Ɏ��S�^�C�}�[���Z�b�g����֐�
	void SetDeathTimer(const float timer);

public://�Q�b�^�[

	const bool GetIsDead();

	bool GetSpawnFlag();

	bool GetDeathAnimationFlag();

	bool GetLockOnFlag();

	const Transform& GetTrans();

private:

	void BulletUpdate(Camera* camera);

	void BulletDraw();

	void SpawnUpdate(Camera* camera, GameTimer* gameTimer);

	void DeathUpdate(Camera* camera, GameTimer* gameTimer);
};