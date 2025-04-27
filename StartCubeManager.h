#pragma once
#include"base.h"
#include"StartCube.h"
#include"Shader.h"
#include<vector>

class StartCubeManager
{
public:
	StartCubeManager(Texture* textureLR, Texture* textureTop, Texture* textureBottom, Texture* textureBack);
	~StartCubeManager();
	//周りの壁の位置・大きさ・shader/textureを設定
	void InitStartCube();
	//描画
	void Render();
	//ビューと投影を設定
	void SetViewProjection(glm::mat4 view, glm::mat4 projection);

private:
	vector<StartCube*> StartCubes;
	glm::mat4 view;					//ビュー
	glm::mat4 projection;			//投影

	Shader* wallShader;
	Texture* textureLR;			//左と右
	Texture* textureTop;		//上
	Texture* textureBottom;	//下
	Texture* textureBack;		//後ろ
};

