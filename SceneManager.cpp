
#include"SceneManager.h"


SceneManager::SceneManager(Scene& gameScene, Scene& titleScene, Scene& gameclearScene)
{
	this->titleScene = &titleScene;
	this->gameScene = &gameScene;
	this->gameclearScene = &gameclearScene;

	SetCurrentScene(this->titleScene);
}
SceneManager::~SceneManager()
{
	titleScene->Terminate();
	gameclearScene->Terminate();
	gameScene->Terminate();
	delete titleScene;
	delete gameScene;
	delete gameclearScene;
}

void SceneManager::SetCurrentScene(Scene* scene)
{
	current_scene = scene;
	current_scene->Enter();
}

void SceneManager::SwitchTo(SceneType type)
{
	current_scene->Exit();
	switch (type)
	{
	case SceneType::Title:
		current_scene = titleScene;
		break;
	case SceneType::Game:
		current_scene = gameScene;
		break;
	case SceneType::Gameclear:
		current_scene = gameclearScene;
		break;
	default:
		break;
	}
	current_scene->Enter();
}

void SceneManager::Update(float deltaTime)
{
	current_scene->Update(deltaTime);
}
void SceneManager::Render()
{
	current_scene->Render();
}


