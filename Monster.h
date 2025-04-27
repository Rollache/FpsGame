#pragma once
#include "base.h"
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

class Monster
{
public:
	enum MonsterType
	{
		Normal = 0,
		Tracking,
		Bird
	};
public:
	Monster() = default;
	virtual ~Monster() = default;
	virtual void Update(float deltaTime, glm::vec3 playerPos) = 0;
	virtual void Render(glm::mat4 view, glm::mat4 projection) = 0;
	virtual bool IsPlayerCollided(glm::vec3 playerPos) = 0;
	virtual glm::vec3 GetPos() = 0;
	//AABB判定用
	virtual glm::vec3 GetMinBound() = 0;
	virtual glm::vec3 GetMaxBound() = 0;

public:

	glm::vec3 GetVelocity() const
	{
		return v;
	}



	bool IsAlive() const
	{
		return alive;
	}

protected:
	Model* monsterModel;
	Shader* monsterShader;
	Texture* monsterTexture;

	glm::mat4 model;
	glm::vec2 windowSize;
	glm::vec3 pos;		//モデル表示用位置（モデルファイルのためここはモデルの中心点ではない）
	glm::vec3 v;			//速度
	glm::vec3 size;     //大きさ
public:
	MonsterType type;	//モンスターの種類
	bool alive = true;		//生きているかどうか
	bool hitPlayer = false;	//プレーヤーに当たったかどうか
};