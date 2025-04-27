#pragma once
#include"base.h"
#include"Scene.h"
#include"Camera.h"

class SceneManager
{
public:
	//シーンの種類
	enum class SceneType
	{
		Title,
		Game,
		Gameclear
	};

public:
	SceneManager(Scene& gameScene, Scene& titleScene, Scene& gameclearScene);

	~SceneManager();
	//現在のシーンを設定
	void SetCurrentScene(Scene* scene);
	//シーンを切り替える
	void SwitchTo(SceneType type);

	void Update(float deltaTime);

	void Render();

private:
	Scene* current_scene = nullptr;
	Scene* titleScene;
	Scene* gameScene;
	Scene* gameclearScene;

public:
	//ゲームを閉じるかどうか
	bool isQuit = false;
	//最新のクリアタイム
	float currentTime = 0.0f;
	//最速クリアタイム
	float bestTime = 0.0f;
};

