#pragma once
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include"MonsterManager.h"
//矢の状態
enum ArrowStatus
{
	idle,
	holding,
	moving
};

class Arrow
{
public:
	Arrow(Shader* arrowShader, Model* arrowModel, Texture* arrowTexture);
	~Arrow() = default;
	//描画
	void Render(glm::mat4 projection, glm::mat4 view);
	//更新処理
	void Update(float deltaTime, Camera& camera, MonsterManager& monsters, bool& isHitTarget);

private:
	//モデル初期化
	void InitModel(Camera& camera);
	//当たり判定（球体）
	bool CheckSphereCollision(glm::vec3 start, glm::vec3 direction, glm::vec3 sphereCenter, float sphereRadius);
	//モンスターとの当たり判定（RAY）
	void CheckCollision(Camera& camera, MonsterManager& monsterManager, bool& isHitTarget);
	//当たり判定（AABB）
	bool CheckAABBCollision(glm::vec3 start, glm::vec3 direction, glm::vec3 minBound, glm::vec3 maxBound);

private:
	glm::mat4 arrowModelMat;
	Shader* arrowShader;
	Model* arrowModel;
	Texture* arrowTexture;

	float baseVelocity = 500.0f;						//速度
	glm::vec3 startPosition;							//撃つ直前の位置
	glm::vec3 direction;									//向き方向
	glm::vec3 movement;								//移動ベクトル
	float gravity;												//重力
	const float ARROWGRAVITY = 0.04f;		//重力
	const float MAXDISTANCE = 10000.0f; // 最大距離　越えたら消される

public:
	float holdTime = 0.0f;								//プレーヤーが弓を引いている時間
	bool isMoving = false;								//矢が飛んでるかどうか
	bool isAlive = true;									//矢が消されたかどうか
	ArrowStatus status = idle;						//矢の状態
	bool onHolding = false;							//引かれているか
};