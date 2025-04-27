#include "Star.h"

Star::Star()
{
	flagModel = glm::mat4(1.0);
	pos = firstPos; //星の初期位置
	isGameclearAnimationOver = false; //ゲームクリアアニメーションの完了フラグを初期化

	LoadModel();
	LoadTexture();
	LoadShader();
}

Star::~Star()
{
	delete flag;
	delete flagShader;
	delete flagTexture;
}

void Star::Update(float deltaTime, const Player& player, bool& isGameClear)
{
	if (!isGameClear)
	{
		//星の回転
		angle += rotateSpeed * deltaTime;
		if (angle >= 360.0f)
		{
			angle -= 360.0f;
		}

		//プレイヤーとの衝突判定
		isGameClear = CheckDiscreteCollision(player.position, player.nextPosition);
	}
}

void Star::UpdateGameClearAnimation(float deltaTime)
{
	//星を高速回転させる
	angle += gameclearRotateSpeed * deltaTime;
	if (angle >= 360.0f)
	{
		angle -= 360.0f;
	}

	//星の移動アニメーション
	if (pos.z >= starMoveGoalZ)
	{
		pos += GameclearAnimationMovespeed;
	}
	else
	{
		isGameclearAnimationOver = true; //アニメーション完了フラグを設定
	}
}

bool Star::CheckDiscreteCollision(glm::vec3 playerPrevPosition, glm::vec3 playerCurrPosition, int steps)
{
	//プレイヤーの移動経路上の複数ポイントで衝突判定を行う
	for (int i = 0; i <= steps; ++i)
	{
		float t = static_cast<float>(i) / steps; //補間比率

		//線形補間でプレイヤーの中間位置を計算
		glm::vec3 interpolatedPos = mix(playerPrevPosition, playerCurrPosition, t);

		//プレイヤーの位置と星の位置の距離が半径以内ならtrue
		if (length(interpolatedPos - pos) <= radius)
		{
			return true;
		}
	}
	return false;
}

void Star::Render(glm::mat4 view, glm::mat4 projection)
{
	flagModel = glm::mat4(1.0);
	flagModel = glm::translate(flagModel, pos); //位置を適用

	//モデル調整
	flagModel = glm::rotate(flagModel, glm::radians(angle), glm::vec3(0.0, 1.0, 0.0));
	flagModel = scale(flagModel, glm::vec3(0.5, 0.5, 1.0));
	flagModel = rotate(flagModel, radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
	flagModel = rotate(flagModel, radians(180.0f), glm::vec3(0.0, 1.0, 0.0));

	flagShader->Bind();

	flagShader->SetMat4("projection", projection);
	flagShader->SetMat4("view", view);
	flagShader->SetMat4("model", flagModel);

	//テクスチャ
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, flagTexture->GetId());

	//描画
	glBindVertexArray(flag->GetVAO());
	glDrawElements(GL_TRIANGLES, static_cast<GLuint>(flag->GetIndices().size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	flagShader->Unbind();
}

bool Star::IsClearAnimationOver()
{
	return isGameclearAnimationOver;
}

void Star::Reset()
{
	pos = firstPos;//初期位置にリセット
	isGameclearAnimationOver = false; //アニメーション完了フラグをリセット
}

void Star::LoadModel()
{
	flag = new Model("res/model/goldstar.fbx");
}

void Star::LoadShader()
{
	flagShader = new Shader("res/shader/star.vert", "res/shader/star.frag");
	flagShader->Bind();
	flagShader->SetInt("diffuse", 0);
	flagShader->Unbind();
}

void Star::LoadTexture()
{
	flagTexture = new Texture("res/texture/gamescene/goldstar.png");
}
