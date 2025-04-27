
#include "ArrowManager.h"

ArrowManager::ArrowManager()
{
	LoadModel();
	LoadTexture();
	LoadShader();
}
ArrowManager::~ArrowManager()
{
	for (Arrow* arrow : arrows)
	{
		delete arrow;
	}
	delete arrowShader;
	delete arrowModel;
	delete arrowTexture;
}

void ArrowManager::Reset()
{
	//holding中の矢を廃棄
	if (arrows.size() > 0)
	{
		arrows[arrows.size() - 1]->status = moving;
	}
}

void ArrowManager::AddArrow()
{
	//新しい矢を追加
	Arrow* arrow = new Arrow(arrowShader, arrowModel, arrowTexture);
	arrow->status = holding;
	arrows.push_back(arrow);
}

void ArrowManager::Update(float deltaTime, Camera& camera, MonsterManager& monsters, bool& isHitTarget)
{
	for (int i = 0; i < arrows.size(); i++)
	{
		arrows[i]->Update(deltaTime, camera, monsters, isHitTarget);
	}
}

void ArrowManager::Render(glm::mat4 projection, glm::mat4 view)
{
	for (int i = 0; i < arrows.size(); i++)
	{
		arrows[i]->Render(projection, view);
	}
}

bool ArrowManager::Shoot()
{
	if (arrows.size() > 0)
	{
		arrows[arrows.size() - 1]->status = moving;
	}
	//溜める時間が0.2s以下になると撃てない
	if (arrows[arrows.size() - 1]->holdTime <= 0.2f)
	{
		return false;
	}
	else
	{
		return true;
	}

}

void ArrowManager::LoadModel()
{
	arrowModel = new Model("res/model/Arrow.obj");
}
void ArrowManager::LoadTexture()
{
	arrowTexture = new Texture("res/texture/gamescene/Arrow_ALB.png");
}
void ArrowManager::LoadShader()
{
	arrowShader = new Shader("res/shader/arrow.vert", "res/shader/arrow.frag");
	arrowShader->Bind();
	arrowShader->SetInt("diffuse", 0);
	arrowShader->Unbind();
}