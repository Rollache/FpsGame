#pragma once
#include<iostream>
#include"Scene.h"
#include"TitleUI.h"
#include"SceneManager.h"

class TitleScene :public Scene
{
public:
	TitleScene(GLFWwindow* window, glm::vec2 windowSize, CharacterTexture& textures, Sound& sound);
	~TitleScene() = default;

	void Terminate() override;

	void Enter() override;

	void Update(float deltaTime) override;

	void Render() override;

	void Exit() override;

	void SetManager(SceneManager& sceneManager);

private:
	TitleUI* titleUI;
	SceneManager* sceneManager;
	bool isEnterPressed = false;
	bool isESCPressed = false;
	bool goNext = false;
};

