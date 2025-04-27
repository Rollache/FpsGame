
#include"TitleScene.h"

TitleScene::TitleScene(GLFWwindow* window, glm::vec2 windowSize, CharacterTexture& textures, Sound& sound)
{
	this->window = window;
	this->windowSize = windowSize;
	this->textures = &textures;
	this->sound = &sound;

	titleUI = new TitleUI(windowSize.x, windowSize.y, textures);
}

void TitleScene::Terminate()
{
	delete titleUI;
}

void TitleScene::Enter()
{
	sound->Play(TITLEBGM);
}

void TitleScene::Update(float deltaTime)
{
	titleUI->Update(deltaTime);
	//enterを押せばゲーム画面に進む
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
	{
		isEnterPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE && isEnterPressed == true)
	{
		sound->Play(OK);
		isEnterPressed = false;
		sceneManager->SwitchTo(SceneManager::SceneType::Game);
	}

	//escを押せば閉じる
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		isESCPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE && isESCPressed == true)
	{
		sound->Play(BACK);
		isESCPressed = false;
		sceneManager->isQuit = true;
	}
}

void TitleScene::Render()
{
	titleUI->render();
}

void TitleScene::Exit()
{
	sound->Stop(TITLEBGM);
}

void TitleScene::SetManager(SceneManager& sceneManager)
{
	this->sceneManager = &sceneManager;
}


