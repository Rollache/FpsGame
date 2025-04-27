#pragma once
#include"base.h"
#include<graphics.h>
#include"Camera.h"
#include"CharacterTexture.h"

class Scene
{
public:
	Scene() = default;
	virtual ~Scene() = default;

	//シーンに入るとき
	virtual void Enter() {}
	//更新処理
	virtual void Update(float deltaTime) {}
	//描画
	virtual void Render() {}
	//シーンを退出するとき
	virtual void Exit() {}
	//メモリ解放のタイミングは自分で管理する
	virtual void Terminate() {}

protected:
	GLFWwindow* window;
	glm::vec2 windowSize;
	CharacterTexture* textures;
	Sound* sound;
};

