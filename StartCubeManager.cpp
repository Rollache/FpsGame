#include "StartCubeManager.h"

StartCubeManager::StartCubeManager(Texture* textureLR, Texture* textureTop, Texture* textureBottom, Texture* textureBack)
{
	//シェーダーをロード
	wallShader = new Shader("res/shader/wall.vert", "res/shader/wall.frag");

	//各面のテクスチャを保存
	this->textureLR = textureLR;     // 左右の壁のテクスチャ
	this->textureTop = textureTop;   // 上のテクスチャ
	this->textureBottom = textureBottom; // 下のテクスチャ
	this->textureBack = textureBack; // 後ろのテクスチャ
}

StartCubeManager::~StartCubeManager()
{
	//すべての StartCube を削除
	for (StartCube* wall : StartCubes)
	{
		delete wall;
	}
	StartCubes.clear();

	//シェーダーを削除
	delete wallShader;
}

void StartCubeManager::InitStartCube()
{
	//上
	StartCube* up = new StartCube(textureTop, glm::vec3(0.0f, 300.0f, -5400.0f),
		glm::vec3(600.0f, 1.0f, 11000.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	StartCubes.push_back(up);

	//右
	StartCube* right = new StartCube(textureLR, glm::vec3(300.0f, 0.0f, -5400.0f),
		glm::vec3(1.0f, 600.0f, 11000.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	StartCubes.push_back(right);

	//左
	StartCube* left = new StartCube(textureLR, glm::vec3(-300.0f, 0.0f, -5400.0f),
		glm::vec3(1.0f, 600.0f, 11000.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	StartCubes.push_back(left);

	//下
	StartCube* bottom = new StartCube(textureBottom, glm::vec3(0.0f, -300.0f, -5400.0f),
		glm::vec3(600.0f, 0.01f, 11000.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	StartCubes.push_back(bottom);

	// 後ろ
	StartCube* back = new StartCube(textureBack, glm::vec3(0.0f, 0.0f, 100.0f),
		glm::vec3(600.0f, 600.0f, 4.0f), 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	StartCubes.push_back(back);
}

void StartCubeManager::Render()
{
	for (int i = 0; i < StartCubes.size(); i++)
	{
		//ビュー・プロジェクション行列を設定
		StartCubes[i]->SetViewProjection(view, projection);

		//描画
		StartCubes[i]->Render(*wallShader);
	}
}

//ビュー・プロジェクション行列を設定
void StartCubeManager::SetViewProjection(glm::mat4 view, glm::mat4 projection)
{
	this->view = view;
	this->projection = projection;
}
