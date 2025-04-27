#include "ExtraFloor.h"

bool ExtraFloor::IsCollision(Player& player)
{
	//プレイヤーの足が床の中にあるかチェック
	if (IsPlayerInsideWithSubsteps(player.GetBottomPos(), player.GetBottomPos() + player.nextPosition))
	{
		isCollided = true; //衝突フラグ

		//プレイヤーのY軸の移動停止
		player.nextPosition.y = 0;

		//プレイヤーのフラグをリセット
		player.isJumped = false;
		player.isDoubleJumped = false;
		player.isBesideLeftWall = false;
		player.isBesideRightWall = false;
		player.isExtraJumped = true;
		player.isQuickDroping = false;

		//移動速度をリセット
		if (player.isWallRided)
		{
			player.isWallRided = false;
			player.movementSpeed = player.SPEED;
		}
		return true; //衝突あり
	}
	//衝突していない
	else
	{
		if (isCollided == true)
		{
			//落下中の場合、重力をリセット
			if (player.gravity < 0.0f)
			{
				player.gravity = 0.0f;
			}

			//衝突フラグを解除
			isCollided = false;
		}
	}
	return false; //衝突なし
}

void ExtraFloor::Render(Shader& shader)
{
	//MVP行列を計算
	glm::mat4 mvp = projection * view * modelMatrix;

	//シェーダーをバインドしてMVP行列を設定
	shader.Bind();
	shader.SetMat4("uMVP", mvp);

	//テクスチャをアクティブにし、シェーダーにセット
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetId());
	shader.SetInt("uTexture", 0);

	//立方体を描画
	glBindVertexArray(Cube::VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//シェーダーのバインドを解除
	shader.Unbind();
}
