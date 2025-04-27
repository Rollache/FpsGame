#pragma once
#include"base.h"
#include"Signpost.h"
#include"Shader.h"
#include "Model.h"
#include "Texture.h"
#include<vector>

class SignpostManager
{
public:
	SignpostManager();
	~SignpostManager();
	//更新処理（色を変える）
	void Update(float deltaTime);
	//描画
	void Render(glm::mat4 view, glm::mat4 projection);

private:
	//初期化関数
	void InitSignpostOnWall();
	void InitSignpostOnFloor();

	void LoadModel();
	void LoadShader();
	void LoadTexture();

private:
	vector<Signpost*> signpostsOnWall;
	vector<Signpost*> signpostsOnFloor;

	Model* signpost;
	Shader* signpostShader;
	Texture* signpostTexture;
	Texture* signpostTextureB;
};