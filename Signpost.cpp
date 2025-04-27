#include "Signpost.h"

Signpost::Signpost(glm::vec3 pos, Model* model, Shader* shader, Texture* signpostTexture, Texture* signpostTextureBlack)
{
	this->pos = pos;
	this->signpost = model;
	this->signpostShader = shader;
	this->signpostTexture = signpostTexture;
	this->signpostTextureBlack = signpostTextureBlack;
}

void Signpost::Update(float deltaTime)
{
	elapsedTime -= deltaTime; //経過時間を減算
	if (elapsedTime <= 0)
	{
		//色を切り替える
		elapsedTime = COUNTDOWNTIME; //カウントダウンをリセット
		isBlack = !isBlack; //色の状態を反転
	}
}

void Signpost::RenderFloor(glm::mat4 view, glm::mat4 projection)
{
	signpostModel = glm::mat4(1.0);
	signpostModel = glm::translate(signpostModel, pos);
	//モデル調整
	signpostModel = scale(signpostModel, glm::vec3(1.0, 1.0, 2.0)); //サイズをスケール調整
	signpostModel = rotate(signpostModel, radians(180.0f), glm::vec3(0.0, 1.0, 0.0)); //180度回転

	signpostShader->Bind(); //シェーダーをバインド

	//行列をシェーダーに適用
	signpostShader->SetMat4("projection", projection);
	signpostShader->SetMat4("view", view);
	signpostShader->SetMat4("model", signpostModel);

	//テクスチャを切り替え
	if (isBlack)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, signpostTextureBlack->GetId());
	}
	else
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, signpostTexture->GetId());
	}

	//モデルを描画
	glBindVertexArray(signpost->GetVAO());
	glDrawElements(GL_TRIANGLES, static_cast<GLuint>(signpost->GetIndices().size()), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	signpostShader->Unbind(); //シェーダーを解除
}

void Signpost::RenderWall(glm::mat4 view, glm::mat4 projection)
{
	signpostModel = glm::mat4(1.0);
	signpostModel = glm::translate(signpostModel, pos);

	//モデル調整
	signpostModel = scale(signpostModel, glm::vec3(10.0, 10.0, 20.0));
	signpostModel = rotate(signpostModel, radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
	signpostModel = rotate(signpostModel, radians(90.0f), glm::vec3(0.0, 0.0, 1.0));

	signpostShader->Bind(); //シェーダーをバインド

	//行列をシェーダーに適用
	signpostShader->SetMat4("projection", projection);
	signpostShader->SetMat4("view", view);
	signpostShader->SetMat4("model", signpostModel);

	//テクスチャを切り替え
	if (isBlack)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, signpostTextureBlack->GetId());
	}
	else
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, signpostTexture->GetId());
	}

	//モデルを描画
	glBindVertexArray(signpost->GetVAO());
	glDrawElements(GL_TRIANGLES, static_cast<GLuint>(signpost->GetIndices().size()), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	signpostShader->Unbind(); //シェーダーを解除
}
