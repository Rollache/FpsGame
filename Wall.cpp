#include "wall.h"

bool Wall::IsWallCollision(Player& player)
{
	//壁の当たり判定：
	//プレイヤーが左または右に移動して壁に衝突した場合、壁走り可能な状態にする
	if (IsPlayerInside(player.leftCollision + player.nextPosition) && player.nextPosition.x < 0)
	{
		player.nextPosition.x = 0;
		player.isBesideLeftWall = true;
		return true;
	}
	else if (IsPlayerInside(player.rightCollision + player.nextPosition) && player.nextPosition.x > 0)
	{
		player.nextPosition.x = 0;
		player.isBesideRightWall = true;
		return true;
	}
	else
	{
		player.isBesideLeftWall = false;
		player.isBesideRightWall = false;
		return false;
	}
}

bool Wall::IsFloorCollision(Player& player)
{
	//プレイヤーの足が床の中にあるかチェック
	if (IsPlayerInsideWithSubsteps(player.GetBottomPos(), player.GetBottomPos() + player.nextPosition))
	{
		//プレイヤーのステータスをリセット
		isOnCollision = true;
		player.nextPosition.y = 0;

		//プレイヤーのフラグをリセット
		player.isJumped = false;
		player.isDoubleJumped = false;
		player.isBesideLeftWall = false;
		player.isBesideRightWall = false;
		player.isExtraJumped = true; //壁走りとダッシュしないと ExtraJump できない
		player.isQuickDroping = false;

		//リセット
		if (player.isWallRided)
		{
			player.isWallRided = false;
			player.movementSpeed = player.SPEED;
		}

		return true;
	}

	//衝突していない場合
	isOnCollision = false;
	return false;
}

void Wall::Render(Shader& shader)
{
	//MVP行列を計算
	glm::mat4 mvp = projection * view * modelMatrix;

	shader.Bind();
	shader.SetMat4("uMVP", mvp);

	//テクスチャをシェーダーにセット
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetId());
	shader.SetInt("uTexture", 0);

	//壁の状態によってシェーダーの色設定を変更
	switch (state)
	{
	case running:
		shader.SetBool("useColor", false);
		break;
	case playerOn:
		shader.SetBool("useColor", true);
		shader.SetVec4("uColor", glm::vec4(0.0f, 0.5f, 0.0f, 0.6f)); //プレイヤーと接触したら色を変える
		break;
	case playerWillBeOn:
		shader.SetBool("useColor", true);
		shader.SetVec4("uColor", glm::vec4(0.0f, 1.0f, 0.0f, 0.3f)); //プレイヤーが上にいたら色を変える
		break;
	default:
		break;
	}

	//壁を描画
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//シェーダーのバインドを解除
	shader.Unbind();
}

void Wall::Update(float velocity, float deltaTime)
{
	//壁をZ軸方向に移動させる
	position.z += velocity * deltaTime;

	//外に出たら戻す
	if (position.z > outOfMapPosZ)
	{
		position.z = firstPosZ;
	}

	//モデル行列を更新
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, rotationAngle, rotationAxis);
	modelMatrix = glm::translate(modelMatrix, position);
	modelMatrix = glm::scale(modelMatrix, scale);
}

void Wall::ChangeType()
{
	if (type == Walls)
	{
		type = Floors;
	}
	else
	{
		type = Walls;
	}
}

void Wall::ChangeColor(Player& player)
{
	//壁のAABBを取得
	std::pair<glm::vec3, glm::vec3> box = GetBoundingBoxWorld();
	float minZ = box.first.z;
	float maxZ = box.second.z;

	//プレイヤーのZ座標によって状態を変更
	if (player.GetPos().z >= minZ && player.GetPos().z <= maxZ)
	{
		if (type == Walls && player.isWallRiding == true)
		{
			state = playerOn;
		}
		else if (type == Floors && isOnCollision == true)
		{
			state = playerOn;
		}
		else
		{
			state = playerWillBeOn;
		}
	}
	else
	{
		state = running;
	}
}
