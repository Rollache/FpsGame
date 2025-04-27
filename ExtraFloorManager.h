#pragma once
#include"base.h"
#include"ExtraFloor.h"
#include"Shader.h"
#include<vector>

class ExtraFloorManager
{
public:
	ExtraFloorManager(Texture* texture, Sound& sound);
	~ExtraFloorManager();

	//壁/床を追加
	void AddFloor
	(
		const glm::vec3& pos = glm::vec3(0.0f, -150.0f, 0.0f),
		const glm::vec3& scale = glm::vec3(100.0f, 4.0f, 300.0f),
		float rotationAngle = 0.0f,
		const glm::vec3& rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f));

	//描画
	void Render();
	//ビューと投影を設定 
	void SetViewProjection(glm::mat4 view, glm::mat4 projection);
	//プレーヤーとの当たり判定（上から下単方向）
	void IsFloorCollision(Player& player);

private:
	glm::mat4 view;				//ビュー
	glm::mat4 projection;		//投影
	vector<ExtraFloor*> floors;
	Shader* floorShader;
	Texture* texture;
	Sound sound;
};