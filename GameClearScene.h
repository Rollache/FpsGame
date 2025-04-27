#pragma once
#include<iostream>
#include"Scene.h"
#include"TitleUI.h"
#include"GameClearUI.h"
#include"SceneManager.h"

class GameClearScene :public Scene
{
public:
	GameClearScene(GLFWwindow* window, glm::vec2 windowSize, CharacterTexture& textures, Sound& sound);
	~GameClearScene() = default;

	void Terminate() override;

	void Enter() override;

	void Update(float deltaTime) override;

	void Render() override;

	void Exit() override;

	void SetManager(SceneManager& sceneManager);

private:
	GameClearUI* gameclearUI;
	SceneManager* sceneManager;
	CharacterTexture* textures;
	bool isEnterPressed = false;
	bool isESCPressed = false;
	bool isSePlayed = false;
};

