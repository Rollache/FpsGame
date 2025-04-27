#pragma once
#include "base.h"
#include "Shader.h"
#include "Texture.h"
#include <vector>
#include <string>

class Skybox
{
public:
	Skybox() = default;
	~Skybox();

	//描画立方体の準備
	void SetFaces(const std::vector<std::string>& faces);
	//初期化
	void Init();
	//描画
	void Render(const glm::mat4& view, const glm::mat4& projection);

private:
	//skybox画像の読み込み
	unsigned int loadCubemap();

private:
	std::vector<std::string> faces;
	GLuint skyboxVAO;
	GLuint skyboxVBO;
	GLuint cubemapTexture;

	Shader* shader;
	float skyboxVertices[108];
};
