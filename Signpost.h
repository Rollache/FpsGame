#pragma once
#include "base.h"
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"

class Signpost
{
public:
	Signpost(glm::vec3 pos, Model* model, Shader* shader, Texture* signpostTexture, Texture* signpostTextureBlack);
	~Signpost() = default;
	//更新処理（色を変える）
	void Update(float deltaTime);
	//横方向で描画
	void RenderFloor(glm::mat4 view, glm::mat4 projection);
	//縦方向で描画
	void RenderWall(glm::mat4 view, glm::mat4 projection);

private:
	float COUNTDOWNTIME = 0.3f;		//色更新時間
	glm::vec3 pos;									//位置
	Model* signpost;
	Shader* signpostShader;
	Texture* signpostTexture;
	Texture* signpostTextureBlack;

	bool isBlack = true;		//ブラックの色をしてるかどうか
	float elapsedTime = COUNTDOWNTIME;		//色更新時間

	glm::mat4 signpostModel;
	float angle = 0.0f;			//角度
};