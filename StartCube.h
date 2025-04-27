#pragma once
#include"base.h"
#include"Cube.h"

class StartCube :public Cube
{
public:
	StartCube
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
	~StartCube() = default;

	//描画
	void Render(Shader& shader);

private:
	Texture* texture;
};

