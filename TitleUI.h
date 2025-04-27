#pragma once
#include"base.h"
#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "UI.h"
#include"CharacterTexture.h"

class TitleUI
{
public:
	TitleUI(float screenWidth, float screenHeight, CharacterTexture& textures);

	~TitleUI();

	void Update(float deltaTime);

	void render();

private:
	void Init();

private:
	Shader* shader;
	glm::mat4 orthoProjection;
	float screenWidth;
	float screenHeight;
	//UIオブジェクト
	UI* bg;
	UI* text;
	UI* name;
	//UIテクスチャ
	Texture* bgTexture;
	Texture* textTexture;
	Texture* nameTexture;
	//テキストの透明度
	float textAlpha;
	//透明度の変化速度
	float alphaUpdateSpeed;
	//文字テクスチャデータ
	CharacterTexture* textures;
};