#include "SceneManager.h"
#include<cassert>

void SceneManager::Update()
{
	scene->Update();
}

void SceneManager::Draw()
{
	scene->Draw();
}

void SceneManager::Destroy()
{
	scene->Destroy();
	delete scene;
}

void SceneManager::ChangeScene(const std::string& sceneName)
{
	assert(sceneFactory);
	assert(nextScene == nullptr);

	//���V�[���𐶐�
	nextScene = sceneFactory->CreateScene(sceneName);

	//���V�[���̗\�񂪂���Ȃ�
	if (nextScene)
	{
		//���V�[���I��
		if (scene)
		{
			scene->Destroy();
			delete scene;
		}

		//�V�[���؂�ւ�
		scene = nextScene;
		nextScene = nullptr;

		//���V�[��������������
		scene->Initialize();
	}
}

void SceneManager::SetSceneFactory(AbstractSceneFactory* sceneFactory)
{
	this->sceneFactory = sceneFactory;
}

SceneManager* SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}