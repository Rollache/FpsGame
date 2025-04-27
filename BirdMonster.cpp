#include "BirdMonster.h"

BirdMonster::BirdMonster(glm::vec3 pos, Model* monsterModel, Shader* monsterShader, Texture* monsterTexture)
{
	model = glm::mat4(1.0); //初期モデル行列を単位行列に設定
	this->pos = pos; //初期位置を設定

	//X軸とY軸の速度をランダムに設定
	this->v.x = rand() % 100 + 50.0f;
	this->v.y = rand() % 100 + 50.0f;
	this->v.z = 240.0f; //Z軸の初期速度を設定
	size = glm::vec3(35.0f, 35.0f, 35.0f);
	alive = true; //生存状態を設定

	//使用するモデル、シェーダー、テクスチャ
	this->monsterModel = monsterModel;
	this->monsterShader = monsterShader;
	this->monsterTexture = monsterTexture;
}

void BirdMonster::Update(float deltaTime, glm::vec3 playerPos)
{
	WallCollision(); //壁との判定
	Move(deltaTime); //移動
}

void BirdMonster::Render(glm::mat4 view, glm::mat4 projection)
{
	//モデル行列を初期化
	model = glm::mat4(1.0);
	model = glm::translate(model, pos); //位置を適用

	//モデルの向きを調整
	model = rotate(model, radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
	model = scale(model, glm::vec3(80, 80, 80)); //スケール

	//シェーダーをバインドして行列を設定
	monsterShader->Bind();
	monsterShader->SetMat4("projection", projection);
	monsterShader->SetMat4("view", view);
	monsterShader->SetMat4("model", model);

	//テクスチャ
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, monsterTexture->GetId());

	//モデルを描画
	glBindVertexArray(monsterModel->GetVAO());
	glDrawElements(GL_TRIANGLES, static_cast<GLuint>(monsterModel->GetIndices().size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//シェーダーのバインドを解除
	monsterShader->Unbind();
}

void BirdMonster::Move(float deltaTime)
{
	//位置を更新
	pos.x += v.x * deltaTime;
	pos.y += v.y * deltaTime;
	pos.z += v.z * deltaTime;
}

//壁との当たり判定
void BirdMonster::WallCollision()
{
	//X軸の壁に衝突した場合、移動方向を反転
	float wallCollisionPosX = 220.0f;
	float wallCollisionPosY = 250.0f;
	float offsetY = 30.0f;//モデルのずれ

	if (pos.x > wallCollisionPosX + size.x / 2)
	{
		pos.x = wallCollisionPosX + size.x / 2;
		v.x = -v.x;
	}
	if (pos.x < -wallCollisionPosX - size.x / 2)
	{
		pos.x = -wallCollisionPosX - size.x / 2;
		v.x = -v.x;
	}

	//Y軸の壁に衝突した場合、移動方向を反転
	if (pos.y > wallCollisionPosY - offsetY + size.y / 2)
	{
		pos.y = wallCollisionPosY - offsetY + size.y / 2;
		v.y = -v.y;
	}
	if (pos.y < -wallCollisionPosY - size.y / 2)
	{
		pos.y = -wallCollisionPosY - size.y / 2;
		v.y = -v.y;
	}
}

//プレイヤーとの当たり判定
bool BirdMonster::IsPlayerCollided(glm::vec3 playerPos)
{
	//プレイヤーとの距離ベクトルを取得
	glm::vec3 distanceVec = GetPos() - playerPos;

	//距離の2乗で判定
	float distanceSquared = glm::dot(distanceVec, distanceVec);
	float radiusSquared = size.x * size.x;

	//プレイヤーとの距離がモンスターの当たり判定半径内ならtrue
	return distanceSquared <= radiusSquared;
}
