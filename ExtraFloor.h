#pragma once
#include"base.h"
#include"Cube.h"
#include"Player.h"

class ExtraFloor :public Cube
{
public:
	ExtraFloor
	(
		Texture* texture,
		const glm::vec3& pos,
		const glm::vec3& scale,
		float rotationAngle = 0.0f,
		const glm::vec3& rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f)
	)
		: Cube(pos, scale, rotationAngle, rotationAxis)
	{
		this->texture = texture;
	}
	~ExtraFloor() = default;

	//当たり判定
	bool IsCollision(Player& player);
	//描画
	void Render(Shader& shader);

private:
	Texture* texture;
	bool isCollided = false;
};