#pragma once
#include "base.h"
#include<vector>
#include "Texture.h"
#include "Shader.h"
#include "Model.h"
#include "Camera.h"
#include"ArrowManager.h"

class Player
{
public:
	Player(glm::vec2 windowSize, GLFWwindow* window, Sound& sound);
	~Player();
	//リセット
	void Reset();
	//更新処理
	void Update(float deltaTime, MonsterManager& monsters);
	//描画
	void Render();
	//updateされて修正された位置を実装
	void UpdatePosition(float wallVelocity, float deltaTime);
	//照準をカメラの位置と共に更新
	void UpdateCrosshair();
	//オープニングアニメーションの前にプレーヤーとカメラの位置を初期化
	void InitStartAnimation();

public:
	glm::vec3 GetPos()
	{
		return position;
	}
	glm::vec3 GetBottomPos()
	{
		return bottomCollision;
	}
	void SetPos(glm::vec3 pos)
	{
		position = pos;
		bottomCollision = position + glm::vec3(0, -HEIGHT, 0);
		topCollision = position;
		leftCollision = position - glm::vec3(WIDTH / 2, 0, 0);
		rightCollision = position + glm::vec3(WIDTH / 2, 0, 0);
	}

	glm::vec3 GetForwardVec()
	{
		return camera.GetForward();
	}

	glm::vec3 GetRightVec()
	{
		return camera.GetRight();
	}

	glm::mat4 GetView()
	{
		return camera.GetViewMatrix();
	}

	glm::mat4 GetProjection()
	{
		return projection;
	}
private:
	void LoadModel();

	void LoadTexture();

	void LoadShader();
	//弓の描画
	void RenderBow();
	//引いている弓の描画
	void RenderShootingBow();
	//マウス移動処理
	void MouseMovement(float deltaTime);
	//キーボード入力処理
	void KeyboardInput(float deltaTime);
	//プレーヤー状態更新
	void ResetPlayerState();
	//照準初期化
	void InitCrosshair();

	void BowUpdate(float deltaTime);

private:
	GLFWwindow* window;
	glm::vec2 windowSize;
	bool firstMouse;
	bool eKeyPressed;
	bool spaceKeyPressed;
	//弓
	glm::mat4 bowModel;
	Model* bow;
	Model* bowShooting;
	Shader* bowShader;
	Texture* bowTexture;
	float bowRecoil;
	//矢
	ArrowManager* arrows;
	//照準
	Billboard* crosshairEffect;
	std::vector<Texture*> crosshairTextures;
	Shader* crosshairShader;
	//プレーヤーのビューと投影
	glm::mat4 projection;
	glm::mat4 view;
	//マウス位置
	double mouseX;
	double mouseY;
	//感度
	float mouseSensitivity;
	//seとbgm
	Sound sound;

public:
	//playerの位置
	glm::vec3 position;
	glm::vec3 nextPosition;
	glm::vec3 bottomCollision;
	glm::vec3 topCollision;
	glm::vec3 leftCollision;
	glm::vec3 rightCollision;
	//playerの状態
	bool isJumped;
	bool isDoubleJumped;
	bool isExtraJumped;
	bool isBesideLeftWall;
	bool isBesideRightWall;
	bool isWallRiding;
	bool isWallRided;
	bool isDashing;
	bool isDashed;
	bool isHitTarget;
	bool isCollsionWithMoveWall;
	bool isCollsionWithExtraFloor;
	bool isQuickDroping;
	//重力
	float gravity;
	//壁走り終わるときの反方向ベクトル
	float wallBounce;
	//移動速度
	float movementSpeed;
	//ダッシュ時間
	float dashTime;

public:
	Camera camera;			//カメラ
	bool isShooting;			//プレーヤーが弓を引いているかどうか
	bool isHolding;				//プレーヤーが弓を引いたかどうか
	bool isAddedArrow;		//プレーヤーが弓を引いたかどうか
	int hp;							//生命値

public:
	const float SPEED = 60.0f;							//歩く速度
	const float RUNSPEED = 100.0f;				//走る速度
	const float RIDESPEED = 500.0f;				//壁走り速度
	const float QUICKDROPSPEED = 80.0f;	//急速落下速度
	const float DASHSPEED = 3000.0f;			//ダッシュ速度
	const float DASHTIME = 0.2f;						//ダッシュ時間
	const float PLAYERGRAVITY = 0.24f;			//重力
	const float JUMPSTRENGTH = 15.0f;		//ジャンプの高さ
	const float WALLBOUNCESTRENGTH = 100.0f;	//壁の反発力
	const float HEIGHT = 10.0f;			//カメラの高さ
	const float WIDTH = 4.0f;				//プレーヤー当たり判定の範囲
};

