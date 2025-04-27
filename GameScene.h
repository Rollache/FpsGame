#pragma once
#include"Scene.h"
#include"World.h"
#include"SceneManager.h"

class GameScene :public Scene
{
public:
	GameScene(GLFWwindow* window, glm::vec2 windowSize, CharacterTexture& textures, Sound& sound);;
	~GameScene() = default;

	void Enter() override;

	void Update(float deltaTime) override;

	void Render() override;

	void Exit() override;

	void Terminate() override;

	void SetManager(SceneManager& sceneManager);

private:
	World* world;
	SceneManager* sceneManager;
	bool isEnterPressed = false;
	bool isESCPressed = false;
	bool isFirst = true;
};

