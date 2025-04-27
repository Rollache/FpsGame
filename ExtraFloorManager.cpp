#include "ExtraFloorManager.h"

ExtraFloorManager::ExtraFloorManager(Texture* texture, Sound& sound)
{
	//テクスチャを設定
	this->texture = texture;

	//シェーダーをロード
	floorShader = new Shader("res/shader/wall.vert", "res/shader/wall.frag");

	//サウンド情報を格納
	this->sound = sound;
}

ExtraFloorManager::~ExtraFloorManager()
{
	//すべての床オブジェクトを削除
	for (ExtraFloor* floor : floors)
	{
		delete floor;
	}
	floors.clear();

	//シェーダーを削除
	delete floorShader;
}

void ExtraFloorManager::AddFloor
(
	const glm::vec3& pos,         //床の位置
	const glm::vec3& scale,       //床のサイズ
	float rotationAngle,          //床の回転角度
	const glm::vec3& rotationAxis //回転軸
)
{
	//新しい床オブジェクトを作成し、リストに追加
	ExtraFloor* floor = new ExtraFloor(texture, pos, scale, rotationAngle, rotationAxis);
	floors.push_back(floor);
}

void ExtraFloorManager::Render()
{
	for (int i = 0; i < floors.size(); i++)
	{
		//各床にビュー・プロジェクション行列を設定
		floors[i]->SetViewProjection(view, projection);

		//床を描画
		floors[i]->Render(*floorShader);
	}
}

void ExtraFloorManager::SetViewProjection(glm::mat4 view, glm::mat4 projection)
{
	this->view = view;
	this->projection = projection;
}

void ExtraFloorManager::IsFloorCollision(Player& player)
{
	for (int i = 0; i < floors.size(); i++)
	{
		//プレイヤーが床と衝突しているか判定
		if (floors[i]->IsCollision(player))
		{
			//もし初めて床に接触した場合、着地音を再生
			if (!player.isCollsionWithExtraFloor)
			{
				sound.Play(JUMPDOWNEXTRA); //床に当たった音を再生
				player.isCollsionWithExtraFloor = true;
			}
			return; //衝突したら即終了　他の床をチェックしない
		}
	}

	//衝突していない場合、フラグをリセット
	player.isCollsionWithExtraFloor = false;
}
