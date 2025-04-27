#pragma once
#include"base.h"
#include "Shader.h"
#include "Texture.h"

class UI
{
public:
	UI(Texture* texture, glm::vec2 pos, glm::vec2 size);

	~UI() = default;
	//描画
	void Render(Shader& shader, const glm::mat4& orthoProjection, float alpha = 1.0f);
	//初期化
	void Init();
	//テクスチャ設定
	void SetTexture(Texture* texture)
	{
		this->texture = texture;
	}
	//画面オフセット（位置）を設定
	void SetOffsetY(float offset)
	{
		offsetY = offset;
	}

	glm::vec2 GetPos()
	{
		return position;
	}

protected:
	Texture* texture;
	glm::vec2 position;
	glm::vec2 size;
	//見えるかどうか
	bool visible;
	float offsetY;
	GLuint VAO, VBO, EBO;
};
