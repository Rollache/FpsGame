#include "wallManager.h"

WallManager::WallManager(Texture* texture, Sound sound)
{
	//シェーダーを読み込む
	wallShader = new Shader("res/shader/wall.vert", "res/shader/wall.frag");

	//テクスチャとサウンドを保存
	this->texture = texture;
	this->sound = sound;
}

WallManager::~WallManager()
{
	//すべての壁オブジェクトを削除
	for (Wall* wall : walls)
	{
		delete wall;
	}
	walls.clear();

	//シェーダーを削除
	delete wallShader;
}

void WallManager::AddWall
(
	float rotationAngle,//回転角度
	const glm::vec3& pos,//位置
	const glm::vec3& scale,//スケール
	const glm::vec3& rotationAxis//回転軸
)
{
	//角度をラジアンに変換
	float angleRadians = glm::radians(rotationAngle);

	//新しい壁オブジェクトを追加
	Wall* wall = new Wall(pos, scale, angleRadians, rotationAxis, texture);
	walls.push_back(wall);
}

void WallManager::Reset()
{
	//すべての壁オブジェクトを削除
	for (auto it = walls.begin(); it != walls.end();)
	{
		delete* it;
		it = walls.erase(it);
	}

	//壁を再生成
	InitWalls();
}

void WallManager::InitWalls()
{
	int prevRotateNum = 3;//前回の回転回数
	int rotateNum = 1;//現在の回転回数
	int numberofWalls = 19;
	float wallInterval = 580.0f;
	glm::vec3 wallFirstPos = glm::vec3(270.0f, 0.0f, -850.0f);//最初の壁の位置

	for (int i = 0; i < numberofWalls; i++)
	{
		//新しい壁の位置を計算
		glm::vec3 wallPos = wallFirstPos;
		wallPos.z -= wallInterval * i;
		//新しい壁を追加
		AddWall(0.0f, wallPos);

		//連続で同じ方向の壁が出ないようにする
		while (rotateNum == prevRotateNum)
		{
			rotateNum = rand() % 3;
		}
		prevRotateNum = rotateNum;

		//指定された回数分だけ壁の種類を変更して回転
		for (int a = 0; a < rotateNum; a++)
		{
			walls[i]->ChangeType();
			walls[i]->RotateAroundZ(-90.0f);
		}
	}
}

void WallManager::Update(Player& player, float deltaTime)
{
	for (int i = 0; i < walls.size(); i++)
	{
		//壁の移動を更新
		walls[i]->Update(m_velocity, deltaTime);

		//プレイヤーとの状態に応じて壁の色を変更
		walls[i]->ChangeColor(player);
	}
}

void WallManager::Render()
{
	for (int i = 0; i < walls.size(); i++)
	{
		//ビュー・プロジェクション行列を適用
		walls[i]->SetViewProjection(view, projection);

		//壁を描画
		walls[i]->Render(*wallShader);
	}
}

void WallManager::SetViewProjection(glm::mat4 view, glm::mat4 projection)
{
	this->view = view;
	this->projection = projection;
}

bool WallManager::IsCollision(Player& player)
{
	for (int i = 0; i < walls.size(); i++)
	{
		//プレイヤーのZ座標が壁と交差していない場合は判定を行わない
		//壁
		if (walls[i]->GetType() == Walls && walls[i]->GetState() != running)
		{
			if (walls[i]->IsWallCollision(player))
			{
				return true;
			}
		}
		//床
		else
		{
			if (walls[i]->IsFloorCollision(player) && walls[i]->GetState() != running)
			{
				//最初の衝突時にサウンドを再生
				if (!player.isCollsionWithMoveWall)
				{
					player.isCollsionWithMoveWall = true;
					sound.Play(JUMPDOWN);
				}
				return true;
			}
		}
	}
	player.isCollsionWithMoveWall = false;
	return false;
}
