#pragma once
#include"BaseScene.h"
#include"InputManager.h"
#include"DrawOversight.h"
#include"Camera.h"
#include"AudioManager.h"

//�^�C�g���V�[��
class ResultScene :public BaseScene
{
private:

	//�N���X�ǂݍ���
	InputManager* input_ = nullptr;
	AudioManager* audioManager_ = nullptr;
	std::unique_ptr<LightManager>lightManager_;

	std::unique_ptr<Camera>camera_;

	//�V��
	std::unique_ptr<Model>skyDome_;
	Transform skyDomeTrans_;
	uint32_t skyDomeTex_;

	myMath::Vector3 cameraPos_;
	float angleX_ = 0.0f;
	float angleY_ = 0.0f;
	const float length_ = 50.0f;

	uint32_t bgm_ = 0;

	uint32_t resultTex_ = 0;
	std::unique_ptr<Sprite>result_;
	myMath::Vector2 resultPos_ = { 0.0f,360.0f };
	float time_ = 0.0f;

public:

	//����������
	void Initialize()override;

	//�I������
	void Destroy()override;

	//�X�V����
	void Update()override;

	//�`�揈��
	void Draw()override;

private:

	void camUpdate();
};