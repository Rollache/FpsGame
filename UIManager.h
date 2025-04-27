#pragma once
#include"base.h"
#include <vector>
#include "Shader.h"
#include "UI.h"
#include"Player.h"
#include "Heart.h"
#include "Action.h"
#include "TimerFont.h"
#include"CharacterTexture.h"

class UIManager
{
public:
	UIManager(float screenWidth, float screenHeight, Player& player, CharacterTexture& textures);
	~UIManager();
	//リセット関数
	void Reset(Player& player);
	//プレーヤーのhpが減るときに使う
	void PlayerHurt(int playerHP);
	//更新処理
	void Update(float deltaTime, const Player& player, float time);
	//描画処理
	void Render(float time);
	//CharacterTextureを使ってアルファベットの描画
	void RenderGameOverText();

private:
	//hpのアイコンを初期化
	void InitHearts(int playerHP);
	//hpのアイコンを描画
	void RenderHearts();
	//プレーヤーアクションのアイコンの初期化
	void InitPlayerAction();
	//プレーヤーアクションのアイコンの描画
	void RenderPlayerAction();
	//プレーヤーアクションのアイコンの更新
	void UpdatePlayerAction(const Player& player);
	//経過時間表示文字を初期化
	void InitTimerFont();
	//経過時間表示文字を変える
	void ChangeTimerNumTexture(int num, TimerFont* timeNum);
	//経過時間表示を更新
	void UpdateTimerNum(float time);
	//経過時間表示を描画
	void RenderTimer(float time);
	//ゲームオーバーの表示文字の初期化
	void InitGameOverText();
	//テクスチャ読み込み
	void LoadTexture();

private:
	std::vector<Heart*> hearts;
	std::vector<Action*> actionUIs;
	std::vector<TimerFont*> timeFonts;
	Shader* shader;
	glm::mat4 orthoProjection;
	float screenWidth;
	float screenHeight;
	//画面が赤くなるかどうか
	bool isRedScreen = false;
	//プレーヤーhpを表示するテクスチャ
	Texture* redHeartTexture;
	Texture* grayHeartTexture;
	//プレーヤーアクションを表示するテクスチャ
	Texture* jumpUITexture;
	Texture* nojumpUITexture;
	Texture* dashUITexture;
	Texture* nodashUITexture;
	Texture* rideUITexture;
	Texture* norideUITexture;
	Texture* aKeyTexture;
	Texture* dKeyTexture;
	Texture* spaceKeyTexture;
	Texture* rightClickTexture;
	//ゲームオーバー表示用
	UI* gameoverTextUI;
	Texture* gameoverText;
private:
	//プレーヤーアクション
	Action* jumpUI;
	Action* dashUI;
	Action* rideUI;
	Action* aKeyUI;
	Action* dKeyUI;
	Action* spaceKeyJumpUI;
	Action* spaceKeyRideUI;
	Action* rightClickUI;
	//時間表示文字
	TimerFont* hundredsplaceNum;
	TimerFont* tensplaceNum;
	TimerFont* onesplaceNum;
	TimerFont* tenthsplaceNum;
	TimerFont* hundredthsplaceNum;
	//プレーヤーが右より左の壁に近いかどうか
	bool isPlayerLeft = true;
	//アルファベットと数字のテクスチャ
	CharacterTexture* textures;
};
