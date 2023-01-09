#include "TitleScene.h"
#include"GameScene.h"

void TitleScene::Initialize()
{
	input = InputManager::GetInstance();
	sceneManager = SceneManager::GetInstance();
}

void TitleScene::Update()
{
	if (input->KeyboardTriggerPush(DIK_SPACE) || input->ControllerButtonTriggerPush(A))
	{
		BaseScene* scene = new GameScene();
		sceneManager->SetNextScene(scene);
	}
}

void TitleScene::Draw()
{
}

void TitleScene::Destroy()
{
}