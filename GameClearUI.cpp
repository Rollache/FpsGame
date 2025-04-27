#include "GameClearUI.h"

GameClearUI::GameClearUI(float screenWidth, float screenHeight, CharacterTexture& textures)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->textures = &textures;
	isAnimationOver = false;

	shader = new Shader("res/shader/UI.vert", "res/shader/UI.frag");
	orthoProjection = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f);
	textAlpha = 1.0f;
	alphaUpdateSpeed = -0.01f;
	starOffsetY = 0.0f;

	Init(textures);
	InitCurTimerFont();
	InitBestTimerFont();
}

GameClearUI::~GameClearUI()
{
	delete bgTexture;
	delete textTexture;
	delete starTexture;
	delete shader;
	for (TimerFont* font : bestTimeFonts)
	{
		delete font;
	}
	for (TimerFont* font : currentTimeFonts)
	{
		delete font;
	}
	delete currentTimeHundredsplaceNum;
	delete currentTimeTensplaceNum;
	delete currentTimeOnesplaceNum;
	delete currentTimeTenthsplaceNum;
	delete currentTimeHundredthsplaceNum;
	delete bestTimeHundredsplaceNum;
	delete bestTimeTensplaceNum;
	delete bestTimeOnesplaceNum;
	delete bestTimeTenthsplaceNum;
	delete bestTimeHundredthsplaceNum;

	delete bg;
	delete text;
	delete star;
}

void GameClearUI::Update(float deltaTime)
{
	//出たり消えたりの処理
	textAlpha += alphaUpdateSpeed;
	if (textAlpha >= 1.0f || textAlpha < -0.0f)
	{
		alphaUpdateSpeed = -alphaUpdateSpeed;
	}
	//星の上昇するアニメーション更新
	if (starOffsetY > -screenHeight * 0.9f)
	{
		starOffsetY -= 15.0f;
		star->SetOffsetY(starOffsetY);
	}
	else
	{
		isAnimationOver = true;
	}
}

void GameClearUI::render(float bestTime, float currentTime)
{
	shader->Bind();
	bg->Render(*shader, orthoProjection);
	glDisable(GL_DEPTH_TEST);
	star->Render(*shader, orthoProjection);
	text->Render(*shader, orthoProjection, textAlpha);
	if (isAnimationOver == true)
	{
		RenderTimer(bestTime, currentTime);
	}
	shader->Unbind();
}

void GameClearUI::Reset()
{
	starOffsetY = 0.0f;
	star->SetOffsetY(starOffsetY);
	isAnimationOver = false;
}

void GameClearUI::UpdateTimerNum(float bestTime, float currentTime)
{
	//時間表示を更新
	int hundredsplace = static_cast<int>(currentTime / 100);
	int tensplace = static_cast<int>(currentTime / 10) % 10;
	int onesplace = static_cast<int>(currentTime) % 10;
	int tenthsplace = static_cast<int>(currentTime * 10) % 10;
	int hundredthsplace = static_cast<int>(currentTime * 100) % 10;
	ChangeTimerNumTextureWhite(hundredsplace, currentTimeHundredsplaceNum);
	ChangeTimerNumTextureWhite(tensplace, currentTimeTensplaceNum);
	ChangeTimerNumTextureWhite(onesplace, currentTimeOnesplaceNum);
	ChangeTimerNumTextureWhite(tenthsplace, currentTimeTenthsplaceNum);
	ChangeTimerNumTextureWhite(hundredthsplace, currentTimeHundredthsplaceNum);

	hundredsplace = static_cast<int>(bestTime / 100);
	tensplace = static_cast<int>(bestTime / 10) % 10;
	onesplace = static_cast<int>(bestTime) % 10;
	tenthsplace = static_cast<int>(bestTime * 10) % 10;
	hundredthsplace = static_cast<int>(bestTime * 100) % 10;
	ChangeTimerNumTextureYellow(hundredsplace, bestTimeHundredsplaceNum);
	ChangeTimerNumTextureYellow(tensplace, bestTimeTensplaceNum);
	ChangeTimerNumTextureYellow(onesplace, bestTimeOnesplaceNum);
	ChangeTimerNumTextureYellow(tenthsplace, bestTimeTenthsplaceNum);
	ChangeTimerNumTextureYellow(hundredthsplace, bestTimeHundredthsplaceNum);
}

void GameClearUI::Init(CharacterTexture& textures)
{
	bgTexture = new Texture("res/texture/clearscene/bg.png");
	bg = new UI(bgTexture, glm::vec2(0, 0), glm::vec2(screenWidth, screenHeight));

	textTexture = new Texture("res/texture/clearscene/clearText.png");
	text = new UI(textTexture, glm::vec2(screenWidth / 2 - 400, screenHeight - 450), glm::vec2(800, 600));

	starTexture = new Texture("res/texture/clearscene/star.png");
	star = new UI(starTexture, glm::vec2(screenWidth / 2 - 240, screenHeight), glm::vec2(480, 300));//1.6倍
}

void GameClearUI::InitBestTimerFont()
{
	//大きさ
	float UIsizeX = 60.0f; //1.3倍
	float UIsizeY = 78.0f;
	//画面offset
	float offsetX = screenWidth / 2 - 8 * UIsizeX;
	float offsetY = screenHeight * 0.7;
	//字と字の距離
	float minusX = 0.0f;

	//BEST
	TimerFont* B = new TimerFont(textures->textureByellow, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	bestTimeFonts.push_back(B);
	minusX -= UIsizeX;
	TimerFont* E = new TimerFont(textures->textureEyellow, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	bestTimeFonts.push_back(E);
	minusX -= UIsizeX;
	TimerFont* S = new TimerFont(textures->textureSyellow, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	bestTimeFonts.push_back(S);
	minusX -= UIsizeX;
	TimerFont* T = new TimerFont(textures->textureTyellow, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	bestTimeFonts.push_back(T);
	minusX -= UIsizeX;

	//TIME
	minusX -= UIsizeX;
	TimerFont* T2 = new TimerFont(textures->textureTyellow, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	bestTimeFonts.push_back(T2);
	minusX -= UIsizeX;
	TimerFont* I = new TimerFont(textures->textureIyellow, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	bestTimeFonts.push_back(I);
	minusX -= UIsizeX;
	TimerFont* M = new TimerFont(textures->textureMyellow, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	bestTimeFonts.push_back(M);
	minusX -= UIsizeX;
	TimerFont* E2 = new TimerFont(textures->textureEyellow, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	bestTimeFonts.push_back(E2);
	minusX -= UIsizeX;
	// :
	TimerFont* colon = new TimerFont(textures->textureColonyellow, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	bestTimeFonts.push_back(colon);
	minusX -= UIsizeX;

	//数字
	bestTimeHundredsplaceNum = new TimerFont(textures->texture0yellow, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	minusX -= UIsizeX;
	bestTimeTensplaceNum = new TimerFont(textures->texture0yellow, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	minusX -= UIsizeX;
	bestTimeOnesplaceNum = new TimerFont(textures->texture0yellow, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	minusX -= UIsizeX;
	//.
	TimerFont* point = new TimerFont(textures->texturePointyellow, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	bestTimeFonts.push_back(point);
	minusX -= UIsizeX;
	//数字
	bestTimeTenthsplaceNum = new TimerFont(textures->texture0yellow, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	minusX -= UIsizeX;
	bestTimeHundredthsplaceNum = new TimerFont(textures->texture0yellow, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	minusX -= UIsizeX;
	//最後のs
	TimerFont* S2 = new TimerFont(textures->textureSyellow, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	bestTimeFonts.push_back(S2);
}

void GameClearUI::InitCurTimerFont()
{
	//大きさ
	float UIsizeX = 40.0f;
	float UIsizeY = 52.0f;
	//画面offset
	float offsetX = screenWidth / 2 - 10 * UIsizeX;
	float offsetY = screenHeight * 0.65 - UIsizeY;
	//字と字の距離
	float minusX = 0.0f;

	//CURRENT
	TimerFont* C = new TimerFont(textures->textureCwhite, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	currentTimeFonts.push_back(C);
	minusX -= UIsizeX;
	TimerFont* U = new TimerFont(textures->textureEwhite, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	currentTimeFonts.push_back(U);
	minusX -= UIsizeX;
	TimerFont* R = new TimerFont(textures->textureRwhite, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	currentTimeFonts.push_back(R);
	minusX -= UIsizeX;
	TimerFont* R2 = new TimerFont(textures->textureRwhite, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	currentTimeFonts.push_back(R2);
	minusX -= UIsizeX;
	TimerFont* E = new TimerFont(textures->textureEwhite, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	currentTimeFonts.push_back(E);
	minusX -= UIsizeX;
	TimerFont* N = new TimerFont(textures->textureNwhite, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	currentTimeFonts.push_back(N);
	minusX -= UIsizeX;
	TimerFont* T = new TimerFont(textures->textureTwhite, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	currentTimeFonts.push_back(T);
	minusX -= UIsizeX;

	//TIME
	minusX -= UIsizeX;
	TimerFont* T2 = new TimerFont(textures->textureTwhite, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	currentTimeFonts.push_back(T2);
	minusX -= UIsizeX;
	TimerFont* I = new TimerFont(textures->textureIwhite, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	currentTimeFonts.push_back(I);
	minusX -= UIsizeX;
	TimerFont* M = new TimerFont(textures->textureMwhite, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	currentTimeFonts.push_back(M);
	minusX -= UIsizeX;
	TimerFont* E2 = new TimerFont(textures->textureEwhite, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	currentTimeFonts.push_back(E2);
	minusX -= UIsizeX;

	//:
	TimerFont* colon = new TimerFont(textures->textureColonwhite, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	currentTimeFonts.push_back(colon);
	minusX -= UIsizeX;

	//数字
	currentTimeHundredsplaceNum = new TimerFont(textures->texture0white, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	minusX -= UIsizeX;
	currentTimeTensplaceNum = new TimerFont(textures->texture0white, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	minusX -= UIsizeX;
	currentTimeOnesplaceNum = new TimerFont(textures->texture0white, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	minusX -= UIsizeX;
	// .
	TimerFont* point = new TimerFont(textures->texturePointwhite, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	currentTimeFonts.push_back(point);
	minusX -= UIsizeX;
	//数字
	currentTimeTenthsplaceNum = new TimerFont(textures->texture0white, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	minusX -= UIsizeX;
	currentTimeHundredthsplaceNum = new TimerFont(textures->texture0white, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	minusX -= UIsizeX;
	//s
	TimerFont* S2 = new TimerFont(textures->textureSwhite, glm::vec2(-minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	currentTimeFonts.push_back(S2);
}

void GameClearUI::ChangeTimerNumTextureYellow(int num, TimerFont* timeNum)
{
	switch (num)
	{
	case 0:
		timeNum->SetTexture(textures->texture0yellow);
		break;
	case 1:
		timeNum->SetTexture(textures->texture1yellow);
		break;
	case 2:
		timeNum->SetTexture(textures->texture2yellow);
		break;
	case 3:
		timeNum->SetTexture(textures->texture3yellow);
		break;
	case 4:
		timeNum->SetTexture(textures->texture4yellow);
		break;
	case 5:
		timeNum->SetTexture(textures->texture5yellow);
		break;
	case 6:
		timeNum->SetTexture(textures->texture6yellow);
		break;
	case 7:
		timeNum->SetTexture(textures->texture7yellow);
		break;
	case 8:
		timeNum->SetTexture(textures->texture8yellow);
		break;
	case 9:
		timeNum->SetTexture(textures->texture9yellow);
		break;
	default:
		break;
	}
}

void GameClearUI::ChangeTimerNumTextureWhite(int num, TimerFont* timeNum)
{
	switch (num)
	{
	case 0:
		timeNum->SetTexture(textures->texture0white);
		break;
	case 1:
		timeNum->SetTexture(textures->texture1white);
		break;
	case 2:
		timeNum->SetTexture(textures->texture2white);
		break;
	case 3:
		timeNum->SetTexture(textures->texture3white);
		break;
	case 4:
		timeNum->SetTexture(textures->texture4white);
		break;
	case 5:
		timeNum->SetTexture(textures->texture5white);
		break;
	case 6:
		timeNum->SetTexture(textures->texture6white);
		break;
	case 7:
		timeNum->SetTexture(textures->texture7white);
		break;
	case 8:
		timeNum->SetTexture(textures->texture8white);
		break;
	case 9:
		timeNum->SetTexture(textures->texture9white);
		break;
	default:
		break;
	}
}

void GameClearUI::RenderTimer(float bestTime, float currentTime)
{
	//currentTimeのフォントを描画
	for (int i = 0; i < currentTimeFonts.size(); i++)
	{
		currentTimeFonts[i]->Render(*shader, orthoProjection);
	}
	if (currentTime >= 100.0f)
	{
		currentTimeHundredsplaceNum->Render(*shader, orthoProjection);
	}
	if (currentTime >= 10.0f)
	{
		currentTimeTensplaceNum->Render(*shader, orthoProjection);
	}
	currentTimeOnesplaceNum->Render(*shader, orthoProjection);
	currentTimeTenthsplaceNum->Render(*shader, orthoProjection);
	currentTimeHundredthsplaceNum->Render(*shader, orthoProjection);

	//bestTimeのフォントを描画
	for (int i = 0; i < bestTimeFonts.size(); i++)
	{
		bestTimeFonts[i]->Render(*shader, orthoProjection);
	}
	if (bestTime >= 100.0f)
	{
		bestTimeHundredsplaceNum->Render(*shader, orthoProjection);
	}
	if (bestTime >= 10.0f)
	{
		bestTimeTensplaceNum->Render(*shader, orthoProjection);
	}
	bestTimeOnesplaceNum->Render(*shader, orthoProjection);
	bestTimeTenthsplaceNum->Render(*shader, orthoProjection);
	bestTimeHundredthsplaceNum->Render(*shader, orthoProjection);
}