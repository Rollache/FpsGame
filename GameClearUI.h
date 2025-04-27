#pragma once
#include"base.h"
#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "UI.h"
#include"TimerFont.h"
#include"CharacterTexture.h"

class GameClearUI
{
public:
	GameClearUI(float screenWidth, float screenHeight, CharacterTexture& textures);

	~GameClearUI();
	//更新処理
	void Update(float deltaTime);
	//描画
	void render(float bestTime, float currentTime);
	//リセット
	void Reset();
	//タイマーを更新
	void UpdateTimerNum(float bestTime, float currentTime);

private:
	//uiの初期化
	void Init(CharacterTexture& textures);
	//ベストタイムのフォントを初期化
	void InitBestTimerFont();
	//今度のタイムのフォントを初期化
	void InitCurTimerFont();
	//数字を黄色に変更
	void ChangeTimerNumTextureYellow(int num, TimerFont* timeNum);
	//数字を白に変更
	void ChangeTimerNumTextureWhite(int num, TimerFont* timeNum);
	//タイマーを描画
	void RenderTimer(float bestTime, float currentTime);

private:
	Shader* shader;
	glm::mat4 orthoProjection;
	float screenWidth;
	float screenHeight;
	//星のy座標オフセット
	float starOffsetY;
	//UIオブジェクト（背景、テキスト、星）
	UI* bg;
	UI* text;
	UI* star;
	// UIテクスチャ
	Texture* bgTexture;
	Texture* textTexture;
	Texture* starTexture;
	// テキストの透明度
	float textAlpha;
	// 透明度の変化速度
	float alphaUpdateSpeed;
	// 文字テクスチャデータ
	CharacterTexture* textures;
	// 現在のタイムフォント
	std::vector<TimerFont*> currentTimeFonts;
	TimerFont* currentTimeHundredsplaceNum;
	TimerFont* currentTimeTensplaceNum;
	TimerFont* currentTimeOnesplaceNum;
	TimerFont* currentTimeTenthsplaceNum;
	TimerFont* currentTimeHundredthsplaceNum;
	// ベストタイムフォント
	std::vector<TimerFont*> bestTimeFonts;
	TimerFont* bestTimeHundredsplaceNum;
	TimerFont* bestTimeTensplaceNum;
	TimerFont* bestTimeOnesplaceNum;
	TimerFont* bestTimeTenthsplaceNum;
	TimerFont* bestTimeHundredthsplaceNum;
public:
	// アニメーションが完了したかどうか
	bool isAnimationOver;
};