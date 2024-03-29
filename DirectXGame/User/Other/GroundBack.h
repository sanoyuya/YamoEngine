#pragma once
#include"DrawOversight.h"
#include"Camera.h"
#include"GameTimer.h"

class GroundBack
{
private:

	std::unique_ptr<Model>groundBack_;
	Transform groundBackTrans_;
	static uint32_t groundBackTex_;

	float alpha_ = 1.0f;

public:

	void Initialize(myMath::Vector2 translation,myMath::Vector2 size);

	void Update(Camera* camera, GameTimer* gameTimer);

	void Draw();

	static void LoadAsset();
};