#pragma once
#include"base.h"
#include "Player.h"
#include "Camera.h"
#include"MonsterManager.h"
#include "Skybox.h"
#include"Shader.h"
#include"ExtraFloorManager.h"
#include"WallManager.h"
#include"StartCubeManager.h"
#include"Star.h"
#include"SignpostManager.h"
#include"UIManager.h"
#include"Timer.h"
#include"CharacterTexture.h"
#include"Billboard.h"

class World
{
public:
	World(GLFWwindow* window, glm::vec2 windowSize, CharacterTexture& characterTextures, Sound& sound);
	~World();
	//リセット
	void Reset();
	//更新処理
	void Update(float deltaTime);
	//描画
	void Render();
	//画面が赤くなる
	void GameOverRender();
	//ゲームクリアアニメーションの描画
	void GameClearRender();
	//ゲームオーバーしてるかどうか
	bool IsGameOver();
	//ゲームクリアのアニメーションを更新
	void PlayGameClearAnimation(float deltaTime);

public:
	bool IsGameClear()
	{
		return isGameClear;
	}

	void SetStartAnimationState(bool flag)
	{
		isStartAnimationPlayed = flag;
	}

	bool IsAnimationOver()
	{
		return star->IsClearAnimationOver();
	}

	float GetElapsedTime()
	{
		return elapsedTime;
	}

	float GetClearCurrentTime()
	{
		return currentTime;
	}

private:
	//フレームバッファを初期化
	void InitFramebuffer();
	//描画する準備
	void InitScreenQuad();
	//スカイボックス初期化
	void InitSkyBox();
	//最初と最後の青い床を初期化
	void InitExtraFloorManager();
	//動く壁の床を初期化
	void InitWallManager();
	//周りの壁の初期化
	void InitStartCubeManager();
	//オープニングアニメーション初期化
	void InitStartAnimation();

	//テクスチャ初期化
	void LoadTexture();
	//shader初期化
	void LoadShader();

	//ゲームの描画
	void GameRender();
	//オープニングアニメーション描画
	void StartAnimationRender();
	//オープニングアニメーション更新
	void StartAnimationUpdate(float deltaTime);
	//ゲーム更新処理
	void GameUpdate(float deltaTime);
private:
	GLFWwindow* window;
	glm::vec2 windowSize;
	//プレーヤー
	Player* player;
	//モンスター
	MonsterManager* monsters;
	//他のオブジェクト
	WallManager* wallManager;
	ExtraFloorManager* extraFloorManager;
	StartCubeManager* startCubeManager;
	Star* star;
	SignpostManager* signpostManager;
	UIManager* uiManager;
	//タイマー
	Timer* timer;
	float elapsedTime = 0.0f;
	float currentTime = 0.0f;
	//オープニングアニメーション
	bool isStartAnimationPlayed;
	float pauseTime;
	//ゲームの状態
	bool isGameOver = false;
	bool isGameClear = false;
	//seとbgm
	Sound sound;
	//スカイボックス
	Skybox skybox;

	//テクスチャ
	Texture* wallTexture;
	Texture* extraFloorTexture;
	Texture* startCubeLRTexture;
	Texture* startCubeTopTexture;
	Texture* startCubeBottomTexture;
	Texture* startCubeBackTexture;
	CharacterTexture* characterTextures;

	//gameoverを表示するため
	GLuint FBO, screenTexture, RBO;
	GLuint screenQuadVAO, screenQuadVBO;
	Shader* screenShader;
};

