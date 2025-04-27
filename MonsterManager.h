#pragma once
#include "base.h"
#include <cstdlib>
#include <ctime>
#include <cmath> 
#include <vector>
#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include"Monster.h"
#include"NormalMonster.h"
#include"TrackingMonster.h"
#include"BirdMonster.h"
#include"Billboard.h"

class MonsterManager
{
public:
	MonsterManager(glm::vec2 windowSize, Sound& sound);
	~MonsterManager();
	//リセット関数
	void Reset();

	//全モンスターの更新処理
	void Update(float deltaTime, glm::vec3 playerPos);

	//モンスターたちとプレーヤーの当たり判定
	bool IsCollided(glm::vec3 playerPos);
	//全モンスターの描画
	void Render(glm::mat4 view, glm::mat4 projection, const Camera& camera);

private:
	//最初のモンスターを追加
	void InitMonsters();
	//モンスター死亡動画テクスチャ
	void InitDeathTextures();
	//モンスターを一体追加
	void AddMonster(float posZ = -11000.0f);

	void LoadModel();

	void LoadShader();

	void LoadTexture();

private:
	//3秒ごとにモンスターを一体追加
	float ADDMONSTERTIME = 3.0f;
	//外に出たら消す位置
	float outOfMapPosZ = 150.0f;
private:
	glm::vec2 windowSize;
	Sound sound;					//音
	float addMonsterTime = ADDMONSTERTIME;		//一定時間ごとにモンスターを一体追加
	//モンスターモデル
	Model* normalMonsterModel;
	Model* trackingMonsterModel;
	Model* birdMonsterModel;
	Shader* monsterShader;
	Texture* normalMonsterTexture;
	Texture* trackingMonsterTexture;
	Texture* birdMonsterTexture;
	//モンスター死亡テクスチャ
	std::vector<Texture*> deathTextures;
	Shader* deathEffectShader;
public:
	vector<Monster*> monsters;
	vector<Billboard*> deathEffects;
};