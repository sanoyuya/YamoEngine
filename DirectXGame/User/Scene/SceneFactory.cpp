#include "SceneFactory.h"
#include"TitleScene.h"
#include"GameScene.h"
#include "EngineOP.h"

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
    //���̃V�[���𐶐�
    BaseScene* newScene = nullptr;

    if (sceneName == "TITLE")
    {
        newScene = new TitleScene();
    }
    else if (sceneName == "GAME")
    {
        newScene = new GameScene();
    }
    else if (sceneName == "EngineOP")
    {
        newScene = new EngineOP();
    }

    return std::unique_ptr<BaseScene>(newScene);
}