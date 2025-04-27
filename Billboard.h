#pragma once
#include"base.h"
#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"

class Billboard
{
public:
	Billboard(glm::vec3 position, float size, std::vector<Texture*> textures, Shader* shader);

	~Billboard();
	//循環更新
	void Update(float deltaTime);
	//最後のフレームまで更新
	void UpdateToLaseFrame(float deltaTime);
	//描画
	void Render(glm::mat4 view, glm::mat4 projection, const Camera& camera);
	//テクスチャを最初からにする
	void ResetTexture();

	void SetPos(vec3 pos)
	{
		this->position = pos;
	}

	void SetFrameDuration(float frameDuration)
	{
		this->frameDuration = frameDuration;
	}

	bool IsAnimationOver() const
	{
		return isOver;
	}

private:
	//描画用の四角形を準備する
	void InitQuad();

private:
	glm::vec3 position;		//位置
	float size;						//大きさ

	int currentFrame;			//現在のテクスチャフレーム
	float elapsedTime;		//経過時間 
	float frameDuration;		//フレーム更新時間
	bool isOver;					//動画プレイ完了したかどうか
	float alpha;					//透明度
	//描画用
	std::vector<Texture*> textures;
	Shader* shader;
	GLuint VAO, VBO, EBO;
};
