#include"TrackingMonster.h"

TrackingMonster::TrackingMonster(glm::vec3 pos, Model* monsterModel, Shader* monsterShader, Texture* monsterTexture)
{
	model = glm::mat4(1.0); //モデル行列の初期化
	this->pos = pos; //初期位置を設定
	this->v.z = 300; //Z軸方向の移動速度
	alive = true; //生存状態設定
	size = glm::vec3(25.0f, 25.0f, 50.0f);

	//初期方向
	forwardDirection = glm::vec3(0.0f, 0.0f, 1.0f);

	//使用するモデル、シェーダー、テクスチャ
	this->monsterModel = monsterModel;
	this->monsterShader = monsterShader;
	this->monsterTexture = monsterTexture;
}

void TrackingMonster::Update(float deltaTime, glm::vec3 playerPos)
{
	Move(deltaTime, playerPos);
}

void TrackingMonster::Render(glm::mat4 view, glm::mat4 projection)
{
	model = glm::mat4(1.0);
	model = glm::translate(model, pos); //モンスターの位置を適用

	//プレイヤーの方向に向かせるための回転計算
	float yaw = atan2(forwardDirection.x, forwardDirection.z); //Y軸の回転角を求める
	float pitch = atan2(forwardDirection.y, sqrt(forwardDirection.x * forwardDirection.x + forwardDirection.z * forwardDirection.z)); //X軸の回転角を求める

	//向きに合わせて回転を適用
	model = rotate(model, -pitch, glm::vec3(1.0, 0.0, 0.0));
	model = rotate(model, yaw, glm::vec3(0.0, 1.0, 0.0));

	//モンスターの初期向きを調整
	model = rotate(model, radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
	model = scale(model, glm::vec3(20, 20, 20)); //スケールを適用

	//シェーダーをバインド
	monsterShader->Bind();

	//各行列をシェーダーに適用
	monsterShader->SetMat4("projection", projection);
	monsterShader->SetMat4("view", view);
	monsterShader->SetMat4("model", model);

	//テクスチャの適用
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, monsterTexture->GetId());

	//モデルの描画
	glBindVertexArray(monsterModel->GetVAO());
	glDrawElements(GL_TRIANGLES, static_cast<GLuint>(monsterModel->GetIndices().size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//シェーダーの解除
	monsterShader->Unbind();
}

void TrackingMonster::Move(float deltaTime, glm::vec3 playerPos)
{
	//プレイヤーが前方にいる場合のみ追跡
	if (pos.z < playerPos.z)
	{
		//現在位置を取得
		glm::vec3 correctedPos = GetPos();
		//速度を計算
		float velocity = sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
		//プレイヤーの方向ベクトルを求める
		glm::vec3 newVec = (correctedPos == playerPos) ? glm::vec3(0.0f) : normalize(playerPos - correctedPos);

		//プレイヤーの方向へ移動
		pos.x += newVec.x * velocity * deltaTime;
		pos.y += newVec.y * velocity * deltaTime;
		pos.z += newVec.z * velocity * deltaTime;

		//移動方向を更新
		forwardDirection = newVec;
	}
	//プレイヤーが後方にいる場合は通常移動
	else
	{
		pos.x += v.x * deltaTime;
		pos.y += v.y * deltaTime;
		pos.z += v.z * deltaTime;
		forwardDirection = glm::vec3(0, 0, 1.0);
	}
}

bool TrackingMonster::IsPlayerCollided(glm::vec3 playerPos)
{
	glm::vec3 minBound = GetPos() - size * 0.5f;
	glm::vec3 maxBound = GetPos() + size * 0.5f;

	//当たってるかどうかをチェック
	bool isInsideX = playerPos.x >= minBound.x && playerPos.x <= maxBound.x;
	bool isInsideY = playerPos.y >= minBound.y && playerPos.y <= maxBound.y;
	bool isInsideZ = playerPos.z >= minBound.z && playerPos.z <= maxBound.z;

	return isInsideX && isInsideY && isInsideZ;
}