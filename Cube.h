#pragma once
#include"base.h"
#include <memory>
#include <iostream>
#include <string>
#include "Texture.h"
#include "Shader.h" 

class Cube
{
public:
	Cube(const glm::vec3& pos, const glm::vec3& scale, float rotationAngle, const glm::vec3& rotationAxis);

	~Cube() = default;

	// ビュー行列と投影行列を設定
	void SetViewProjection(const glm::mat4& v, const glm::mat4& p);

	//描画
	void Render(Shader& shader);

	// AABB当たり判定
	bool CheckCollision(const Cube& other) const;

	//位置を取得
	glm::vec3 GetPos() const { return position; }

	//点（プレーヤー）が立方体の中にいるかどうか
	bool IsPlayerInside(const glm::vec3& point) const;

	//プレーヤーが立方体の中にいるかどうか　をいくつかの距離に分けて判断する
	bool IsPlayerInsideWithSubsteps(const glm::vec3& start, const glm::vec3& end, int substeps = 80) const;

	//Z 轴回転
	void RotateAroundZ(float angleDegrees = 90.0f);

	//立方体のaabbを取得
	std::pair<glm::vec3, glm::vec3> GetBoundingBoxWorld() const;

protected:
	void InitData();

protected:
	glm::vec3 position;				//位置
	glm::vec3 scale;					//大きさ
	float  rotationAngle;				//回転角度
	glm::vec3 rotationAxis;		//回転軸
	glm::mat4 view;					//ビュー
	glm::mat4 projection;			//投影
	glm::mat4 modelMatrix = glm::mat4(1.0f);		//モデル行列

protected:
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
};


