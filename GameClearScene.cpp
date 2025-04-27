
#include"GameClearScene.h"

GameClearScene::GameClearScene(GLFWwindow* window, glm::vec2 windowSize, CharacterTexture& textures, Sound& sound)
{
	this->window = window;
	this->windowSize = windowSize;
	this->textures = &textures;
	this->sound = &sound;

	gameclearUI = new GameClearUI(windowSize.x, windowSize.y, textures);
}

void GameClearScene::Terminate()
{
	delete gameclearUI;
}

void GameClearScene::Enter()
{
	isSePlayed = false;
	gameclearUI->Reset();
	//best timeとcurrent timeの設置
	gameclearUI->UpdateTimerNum(sceneManager->bestTime, sceneManager->currentTime);
	sound->Play(STARUP);
}

void GameClearScene::Update(float deltaTime)
{
	gameclearUI->Update(deltaTime);
	//動画のse
	if (gameclearUI->isAnimationOver && !isSePlayed)
	{
		sound->Stop(STARUP);
		sound->Play(STAROVER);
		isSePlayed = true;
	}
	//enter押せばタイトル画面に戻る
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		isEnterPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE && isEnterPressed == true)
	{
		isEnterPressed = false;
		sceneManager->SwitchTo(SceneManager::SceneType::Title);
	}

	//esc押せばタイトル画面に戻る
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		isESCPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE && isESCPressed == true)
	{
		sound->Play(BACK);
		isESCPressed = false;
		sceneManager->SwitchTo(SceneManager::SceneType::Title);
	}
}

void GameClearScene::Render()
{
	gameclearUI->render(sceneManager->bestTime, sceneManager->currentTime);
}

void GameClearScene::Exit()
{

}

void GameClearScene::SetManager(SceneManager& sceneManager)
{
	this->sceneManager = &sceneManager;
}


