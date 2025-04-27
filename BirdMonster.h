#pragma once
#include"Monster.h"

class BirdMonster :public Monster
{
public:
	BirdMonster(glm::vec3 pos, Model* monsterModel, Shader* monsterShader, Texture* monsterTexture);
	~BirdMonster() = default;
	//更新処理
	void Update(float deltaTime, glm::vec3 playerPos)override;
	//描画
	void Render(glm::mat4 view, glm::mat4 projection)override;
	//モンスターとプレーヤーの当たり判定
	bool IsPlayerCollided(glm::vec3 playerPos)override;
	//ゲーム判定用位置(モデル位置のずれを解消)
	glm::vec3 GetPos() override
	{
		float yOffset = 28;
		return glm::vec3(pos.x, pos.y + yOffset, pos.z);
	}

	glm::vec3 GetMinBound() override
	{
		return GetPos() - size * 0.5f;
	}

	glm::vec3 GetMaxBound() override
	{
		return GetPos() + size * 0.5f;
	}

private:
	//移動
	void Move(float deltaTime);
	//壁との当たり判定
	void WallCollision();

};