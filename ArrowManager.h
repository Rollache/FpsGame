#pragma once
#include"Arrow.h"
#include<vector>
#include "Texture.h"
#include "Shader.h"
#include "Model.h"

class ArrowManager
{
public:
	ArrowManager();
	~ArrowManager();
	//リセット
	void Reset();
	//矢を弓に付ける
	void AddArrow();
	//すべての矢の更新処理
	void Update(float deltaTime, Camera& camera, MonsterManager& monsters, bool& isHitTarget);
	//描画
	void Render(glm::mat4 projection, glm::mat4 view);
	//撃てるかを判断する
	bool Shoot();

private:
	void LoadModel();
	void LoadTexture();
	void LoadShader();

private:
	Shader* arrowShader;
	Model* arrowModel;
	Texture* arrowTexture;
	vector<Arrow*> arrows;
};