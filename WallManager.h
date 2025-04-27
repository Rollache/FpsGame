#pragma once
#include"base.h"
#include"Wall.h"
#include"Shader.h"
#include<vector>

class WallManager
{
public:
	WallManager(Texture* texture, Sound sound);
	~WallManager();

	//リセット
	void Reset();
	//最初の壁/床の位置・大きさ・shader/textureを設定
	void InitWalls();
	//更新処理
	void Update(Player& player, float deltaTime);
	//描画
	void Render();

	//ビューと投影を設定
	void SetViewProjection(glm::mat4 view, glm::mat4 projection);
	//当たり判定
	bool IsCollision(Player& player);

	void SetVelocity(float velocity)
	{
		m_velocity = velocity;
	}

	float GetVelocity()
	{
		return m_velocity;
	}

private:
	//壁を追加
	void AddWall
	(
		float rotationAngle = 0.0f,
		const glm::vec3& pos = glm::vec3(300.0f, 0.0f, -350.0f),
		const glm::vec3& scale = glm::vec3(8.0f, 580.0f, 580.0f),
		const glm::vec3& rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f)
	);

private:
	vector<Wall*> walls;
	glm::mat4 view;
	glm::mat4 projection;

	Shader* wallShader;
	Texture* texture;

	float m_velocity = 120.0f;
	Sound sound;


};