#include "SceneFactory.h"
#include"TitleScene.h"
#include"GameScene.h"
#include "EngineOP.h"
#include"GameClearScene.h"
#include"GameOverScene.h"
#include"DemoScene.h"
#include"StageSelectScene.h"

std::unique_ptr<BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
    //次のシーンを生成
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
    else if (sceneName == "GAMECLEAR")
    {
        newScene = new GameClearScene();
    }
    else if (sceneName == "GAMEOVER")
    {
        newScene = new GameOverScene();
    }
    else if (sceneName == "STAGESELECT")
    {
        newScene = new StageSelectScene();
    }
    else if (sceneName == "DEMO")
    {
        newScene = new DemoScene();
    }

    return std::unique_ptr<BaseScene>(newScene);
}