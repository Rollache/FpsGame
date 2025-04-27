#include"NormalMonster.h"

NormalMonster::NormalMonster(glm::vec3 pos, Model* monsterModel, Shader* monsterShader, Texture* monsterTexture)
{
	model = glm::mat4(1.0); //モデル行列の初期化
	this->pos = pos; //初期位置
	this->v.z = 240.0f; //Z軸方向の移動速度
	size = glm::vec3(30.0f, 30.0f, 60.0f);
	alive = true; //生存状態を設定

	//使用するモデル、シェーダー、テクスチャ
	this->monsterModel = monsterModel;
	this->monsterShader = monsterShader;
	this->monsterTexture = monsterTexture;
}

void NormalMonster::Update(float deltaTime, glm::vec3 playerPos)
{
	Move(deltaTime);
}

void NormalMonster::Render(glm::mat4 view, glm::mat4 projection)
{
	//モデル行列の初期化
	model = glm::mat4(1.0);
	model = glm::translate(model, pos); //モンスターの位置を適用
	model = scale(model, glm::vec3(30, 30, 30));

	//シェーダーをバインド
	monsterShader->Bind();

	//各行列をシェーダーに適用
	monsterShader->SetMat4("projection", projection);
	monsterShader->SetMat4("view", view);
	monsterShader->SetMat4("model", model);

	//テクスチャ
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, monsterTexture->GetId());

	//モデルの描画
	glBindVertexArray(monsterModel->GetVAO());
	glDrawElements(GL_TRIANGLES, static_cast<GLuint>(monsterModel->GetIndices().size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//シェーダーの解除
	monsterShader->Unbind();
}

void NormalMonster::Move(float deltaTime)
{
	pos.x += v.x * deltaTime;
	pos.y += v.y * deltaTime;
	pos.z += v.z * deltaTime;
}

bool NormalMonster::IsPlayerCollided(glm::vec3 playerPos)
{
	glm::vec3 minBound = GetPos() - size * 0.5f;
	glm::vec3 maxBound = GetPos() + size * 0.5f;

	//当たってるかどうかをチェック
	bool isInsideX = playerPos.x >= minBound.x && playerPos.x <= maxBound.x;
	bool isInsideY = playerPos.y >= minBound.y && playerPos.y <= maxBound.y;
	bool isInsideZ = playerPos.z >= minBound.z && playerPos.z <= maxBound.z;

	return isInsideX && isInsideY && isInsideZ;
}
