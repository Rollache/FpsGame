#include "base.h"
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include "Player.h"

class Star
{
public:
	Star();
	~Star();
	//更新処理
	void Update(float deltaTime, const Player& player, bool& isGameClear);
	//ゲームクリアアニメーション更新
	void UpdateGameClearAnimation(float deltaTime);
	//プレーヤーとの当たり判定（step付き）
	bool CheckDiscreteCollision(glm::vec3 playerPrevPosition, glm::vec3 playerCurrPosition, int steps = 40);
	//描画
	void Render(glm::mat4 view, glm::mat4 projection);
	//ゲームクリアアニメーションが終わったかどうかを取得
	bool IsClearAnimationOver();
	//星のリセット
	void Reset();
private:
	void LoadModel();

	void LoadShader();

	void LoadTexture();

private:
	glm::vec3 pos;	//位置
	glm::vec3 firstPos = glm::vec3(0.0, -130.0, -10300.0);	//最初の位置

	Model* flag;		//モデル
	Shader* flagShader;
	Texture* flagTexture;
	glm::mat4 flagModel;
	float angle = 0.0f;			//回転角度
	float radius = 40.0f;		//半径
	float rotateSpeed = 50.0f;  //回転速度
	float gameclearRotateSpeed = 800.0f;

	//アニメションプレー完了判定用
	float starMoveGoalZ = -14000.0f;
	glm::vec3 GameclearAnimationMovespeed = glm::vec3(0.0f, 30.0f, -50.0f);
	bool isGameclearAnimationOver;//ゲームクリアアニメーションが終わったかどうか
};