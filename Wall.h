#pragma once
#include"base.h"
#include"Cube.h"
#include"Player.h"

enum WallType
{
	Walls = 0,
	Floors
};
//プレーヤーが壁/床の上にいると色が変わるために
enum WallState
{
	running = 0,
	playerOn,
	playerWillBeOn
};

class Wall :public Cube
{
public:
	Wall(const glm::vec3& pos, const glm::vec3& scale, float rotationAngle, const glm::vec3& rotationAxis, Texture* texture)
		: Cube(pos, scale, rotationAngle, rotationAxis)
	{
		this->texture = texture;
	}
	~Wall() = default;

	//壁の当たり判定
	bool IsWallCollision(Player& player);
	//床の当たり判定
	bool IsFloorCollision(Player& player);
	//描画
	void Render(Shader& shader);
	//更新処理
	void Update(float velocity, float deltaTime);

	void ChangeType();
	//色を変える
	void ChangeColor(Player& player);

	WallType GetType()
	{
		return type;
	}

	WallState GetState()
	{
		return state;
	}

private:
	WallType type = Walls;
	Texture* texture;
	WallState state = running;
	bool isOnCollision = false;

	//外に出る位置
	float outOfMapPosZ = 619.5f;
	//外に出たら戻る位置
	float firstPosZ = -10400.0f;
};