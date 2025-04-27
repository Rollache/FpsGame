
#include"gameScene.h"


GameScene::GameScene(GLFWwindow* window, glm::vec2 windowSize, CharacterTexture& textures, Sound& sound)
{
	this->window = window;
	this->windowSize = windowSize;
	this->textures = &textures;
	this->sound = &sound;
	world = new World(window, windowSize, textures, sound);
}

void GameScene::Enter()
{
	world->SetStartAnimationState(false);
	world->Reset();
	sound->Play(GAMEBGM);
}

void GameScene::Update(float deltaTime)
{
	if (!world->IsGameClear())
	{
		//ゲームオーバーしない限り更新処理を行う
		if (!world->IsGameOver())
		{
			world->Update(deltaTime);
		}
		//ゲームオーバー処理
		else
		{
			sound->Stop(GAMEBGM);
			if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
			{
				isEnterPressed = true;
			}
			if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_RELEASE && isEnterPressed == true)
			{
				isEnterPressed = false;
				world->Reset();
				sound->Play(GAMEBGM);
			}
		}
	}
	//ゲームクリア処理
	else
	{
		//アニメーションが終わっていなければアニメーションを更新
		if (!world->IsAnimationOver())
		{
			world->PlayGameClearAnimation(deltaTime);
		}
		//アニメーションが終わったらクリア時間を読み取ってシーン遷移
		else
		{
			sceneManager->currentTime = world->GetClearCurrentTime();
			//best time更新
			if (sceneManager->bestTime > sceneManager->currentTime || sceneManager->bestTime == 0.0f)
			{
				sceneManager->bestTime = sceneManager->currentTime;
			}
			sound->Stop(GAMEBGM);
			sceneManager->SwitchTo(SceneManager::SceneType::Gameclear);
		}
	}
	//escを押せばタイトル画面に戻る
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		isESCPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE && isESCPressed == true)
	{
		sound->Play(BACK);
		isESCPressed = false;
		sound->Stop(GAMEBGM);
		sceneManager->SwitchTo(SceneManager::SceneType::Title);
	}
}

void GameScene::Render()
{
	if (!world->IsGameOver() && !world->IsGameClear())
	{
		world->Render();
	}
	if (world->IsGameOver())
	{
		world->GameOverRender();
	}
	if (world->IsGameClear())
	{
		world->GameClearRender();
	}
}

void GameScene::Exit()
{

}

void GameScene::Terminate()
{
	delete world;
}

void GameScene::SetManager(SceneManager& sceneManager)
{
	this->sceneManager = &sceneManager;
}
