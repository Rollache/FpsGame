#include "SignpostManager.h"

SignpostManager::SignpostManager()
{
	LoadModel();
	LoadTexture();
	LoadShader();

	InitSignpostOnWall();
	InitSignpostOnFloor();
}

SignpostManager::~SignpostManager()
{
	for (Signpost* signpost : signpostsOnFloor)
	{
		delete signpost;
	}
	for (Signpost* signpost : signpostsOnWall)
	{
		delete signpost;
	}

	//モデル、シェーダー、テクスチャのメモリを解放
	delete signpost;
	delete signpostShader;
	delete signpostTexture;
	delete signpostTextureB;
}

void SignpostManager::InitSignpostOnWall()
{
	int firstSignpostZPos = -2000; //最初のSignpostのz軸の位置
	int signpostIntervalZ = 4000; //各Signpostのz軸の距離

	int posz = firstSignpostZPos; //最初のZ座標

	for (int i = 1; i < 4; i++)
	{
		//右側の壁に配置
		Signpost* signpostOnWallRight = new Signpost(glm::vec3(280.0, -50.0, posz), signpost, signpostShader, signpostTexture, signpostTextureB);
		signpostsOnWall.push_back(signpostOnWallRight);

		//左側の壁に配置
		Signpost* signpostOnWallLeft = new Signpost(glm::vec3(-280.0, -50.0, posz), signpost, signpostShader, signpostTexture, signpostTextureB);
		signpostsOnWall.push_back(signpostOnWallLeft);

		//Z座標を移動
		posz -= signpostIntervalZ;
	}
}

void SignpostManager::InitSignpostOnFloor()
{
	//床に1つ配置
	Signpost* signpostOnFloor1 = new Signpost(glm::vec3(0.0, -148.0, -120.0), signpost, signpostShader, signpostTexture, signpostTextureB);
	signpostsOnFloor.push_back(signpostOnFloor1);
}

void SignpostManager::Update(float deltaTime)
{
	for (int i = 0; i < signpostsOnWall.size(); i++)
	{
		signpostsOnWall[i]->Update(deltaTime);
	}

	for (int i = 0; i < signpostsOnFloor.size(); i++)
	{
		signpostsOnFloor[i]->Update(deltaTime);
	}
}

void SignpostManager::Render(glm::mat4 view, glm::mat4 projection)
{
	for (int i = 0; i < signpostsOnWall.size(); i++)
	{
		signpostsOnWall[i]->RenderWall(view, projection);
	}

	for (int i = 0; i < signpostsOnFloor.size(); i++)
	{
		signpostsOnFloor[i]->RenderFloor(view, projection);
	}
}

void SignpostManager::LoadModel()
{
	signpost = new Model("res/model/signpost.obj");
}

void SignpostManager::LoadShader()
{
	signpostShader = new Shader("res/shader/bow.vert", "res/shader/bow.frag");
	signpostShader->Bind();
	signpostShader->SetInt("diffuse", 0);
	signpostShader->Unbind();
}

void SignpostManager::LoadTexture()
{
	signpostTexture = new Texture("res/texture/gamescene/signpost.png"); //通常のテクスチャ
	signpostTextureB = new Texture("res/texture/gamescene/signpostB.png"); //黒色のテクスチャ
}
