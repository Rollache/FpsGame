#include"UIManager.h"

UIManager::UIManager(float screenWidth, float screenHeight, Player& player, CharacterTexture& textures)
{
	this->textures = &textures;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	shader = new Shader("res/shader/UI.vert", "res/shader/UI.frag");
	orthoProjection = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f);

	LoadTexture();

	InitHearts(player.hp);
	InitPlayerAction();
	InitTimerFont();
	InitGameOverText();
}

UIManager::~UIManager()
{
	for (int i = 0; i < timeFonts.size(); i++)
	{
		delete timeFonts[i];
	}
	for (int i = 0; i < hearts.size(); i++)
	{
		delete hearts[i];
	}
	for (int i = 0; i < actionUIs.size(); i++)
	{
		delete actionUIs[i];
	}
	delete shader;

	delete aKeyUI;
	delete dKeyUI;
	delete spaceKeyJumpUI;
	delete spaceKeyRideUI;
	delete rightClickUI;

	delete redHeartTexture;
	delete grayHeartTexture;
	delete jumpUITexture;
	delete nojumpUITexture;
	delete dashUITexture;
	delete nodashUITexture;
	delete rideUITexture;
	delete norideUITexture;
	delete aKeyTexture;
	delete dKeyTexture;
	delete spaceKeyTexture;
	delete rightClickTexture;

	delete hundredsplaceNum;
	delete tensplaceNum;
	delete onesplaceNum;
	delete tenthsplaceNum;
	delete hundredthsplaceNum;

	delete gameoverText;
	delete gameoverTextUI;
}

void UIManager::Reset(Player& player)
{
	for (auto heart : hearts)
	{
		delete heart;
	}
	hearts.clear();

	isRedScreen = false;
	InitHearts(player.hp);
}

void UIManager::PlayerHurt(int playerHP)
{
	//プレーヤーがダメージを受けたらHP表示を更新する
	for (int i = 0; i < 3; i++)
	{
		if (i >= playerHP)
		{
			hearts[i]->SetTexture(grayHeartTexture);
		}
	}
}

void UIManager::Update(float deltaTime, const Player& player, float time)
{
	//プレーヤーアクションのアイコンを更新
	UpdatePlayerAction(player);
	isPlayerLeft = (player.position.x <= 0);
	//時間表示を更新
	UpdateTimerNum(time);
	//ゲームオーバーしたら画面が赤くなる
	if (player.hp <= 0)
	{
		isRedScreen = true;
	}
}

void UIManager::Render(float time)
{
	shader->Bind();
	//プレーヤーHP
	RenderHearts();
	//プレーヤーアクションアイコン
	RenderPlayerAction();
	//経過時間表示
	RenderTimer(time);

	shader->Unbind();
}

void UIManager::RenderGameOverText()
{
	//GAMEOVERのフォントの表示
	gameoverTextUI->Render(*shader, orthoProjection);
}

void UIManager::InitHearts(int playerHP)
{
	float posX = 0.0f;
	float interval = 100.0f;
	glm::vec2 size = glm::vec2(100.0f, 100.0f);
	for (int i = 0; i < playerHP; i++)
	{
		Heart* heart = new Heart(redHeartTexture, glm::vec2(posX, 0.0f), size);
		hearts.push_back(heart);
		posX += interval;
	}
}

void UIManager::RenderHearts()
{
	for (int i = 0; i < hearts.size(); i++)
	{
		hearts[i]->Render(*shader, orthoProjection);
	}
}

void UIManager::InitPlayerAction()
{
	float UIsizeX = 150.0f;
	float UIsizeY = 150.0f;
	float keysizeX = 100.0f;
	float keysizeY = 100.0f;
	float offsetX = 80.0f;
	float offsetY = 20.0f;

	float minusY = -screenHeight / 5;

	float xOffset = 50.0f; //画面左部分の距離
	float yInterval = 200.0f; //uiの距離

	jumpUI = new Action(jumpUITexture, glm::vec2(xOffset, screenHeight - UIsizeY + minusY), glm::vec2(UIsizeX, UIsizeY));
	actionUIs.push_back(jumpUI);
	//ジャンプのspace
	spaceKeyJumpUI = new Action(spaceKeyTexture, glm::vec2(xOffset + UIsizeX, screenHeight - UIsizeY + minusY + offsetY), glm::vec2(keysizeX, keysizeY));
	minusY -= yInterval;

	dashUI = new Action(dashUITexture, glm::vec2(xOffset, screenHeight - UIsizeY + minusY), glm::vec2(UIsizeX, UIsizeY));
	actionUIs.push_back(dashUI);
	//ダッシュの右クリック
	rightClickUI = new Action(rightClickTexture, glm::vec2(xOffset + UIsizeX, screenHeight - UIsizeY + minusY + offsetY), glm::vec2(keysizeX, keysizeY));
	minusY -= yInterval;

	rideUI = new Action(rideUITexture, glm::vec2(xOffset, screenHeight - UIsizeY + minusY), glm::vec2(UIsizeX, UIsizeY));
	actionUIs.push_back(rideUI);

	//adとspace
	aKeyUI = new Action(aKeyTexture, glm::vec2(xOffset + UIsizeX, screenHeight - UIsizeY + minusY + offsetY), glm::vec2(keysizeX, keysizeY));
	dKeyUI = new Action(dKeyTexture, glm::vec2(xOffset + UIsizeX, screenHeight - UIsizeY + minusY + offsetY), glm::vec2(keysizeX, keysizeY));
	spaceKeyRideUI = new Action(spaceKeyTexture, glm::vec2(xOffset + UIsizeX + offsetX, screenHeight - UIsizeY + minusY + offsetY), glm::vec2(keysizeX, keysizeY));

}

void UIManager::RenderPlayerAction()
{
	//アクションアイコンの表示
	for (int i = 0; i < actionUIs.size(); i++)
	{
		actionUIs[i]->Render(*shader, orthoProjection);
	}
	//キーの表示
	//プレーヤーが左の壁に近ければ壁走りのKEYはAにする
	if (isPlayerLeft)
	{
		aKeyUI->Render(*shader, orthoProjection);
	}
	else
	{
		dKeyUI->Render(*shader, orthoProjection);
	}
	spaceKeyJumpUI->Render(*shader, orthoProjection);
	spaceKeyRideUI->Render(*shader, orthoProjection);
	rightClickUI->Render(*shader, orthoProjection);
}

void UIManager::UpdatePlayerAction(const Player& player)
{

	if (!player.isJumped || !player.isDoubleJumped || !player.isExtraJumped)
	{
		jumpUI->SetTexture(jumpUITexture);
	}
	else
	{
		jumpUI->SetTexture(nojumpUITexture);
	}

	if (!player.isWallRided)
	{
		rideUI->SetTexture(rideUITexture);
	}
	else
	{
		rideUI->SetTexture(norideUITexture);
	}

	if (!player.isDashed)
	{
		dashUI->SetTexture(dashUITexture);
	}
	else
	{
		dashUI->SetTexture(nodashUITexture);
	}
}

void UIManager::InitTimerFont()
{
	//大きさ
	float UIsizeX = 30.0f;
	float UIsizeY = 50.0f;
	//画面offset
	float offsetX = -80.0f;
	float offsetY = 20.0f;
	//字と字の距離
	float minusX = 0.0f;
	//字の数
	int numofFont = 12;


	TimerFont* T = new TimerFont(textures->textureTyellow, glm::vec2(screenWidth - UIsizeX * numofFont - minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	timeFonts.push_back(T);
	minusX -= UIsizeX;
	TimerFont* I = new TimerFont(textures->textureIyellow, glm::vec2(screenWidth - UIsizeX * numofFont - minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	timeFonts.push_back(I);
	minusX -= UIsizeX;
	TimerFont* M = new TimerFont(textures->textureMyellow, glm::vec2(screenWidth - UIsizeX * numofFont - minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	timeFonts.push_back(M);
	minusX -= UIsizeX;
	TimerFont* E = new TimerFont(textures->textureEyellow, glm::vec2(screenWidth - UIsizeX * numofFont - minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	timeFonts.push_back(E);
	minusX -= UIsizeX;
	//:
	TimerFont* colon = new TimerFont(textures->textureColonyellow, glm::vec2(screenWidth - UIsizeX * numofFont - minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	timeFonts.push_back(colon);
	minusX -= UIsizeX;
	//数字
	hundredsplaceNum = new TimerFont(textures->texture0yellow, glm::vec2(screenWidth - UIsizeX * numofFont - minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	minusX -= UIsizeX;
	tensplaceNum = new TimerFont(textures->texture0yellow, glm::vec2(screenWidth - UIsizeX * numofFont - minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	minusX -= UIsizeX;
	onesplaceNum = new TimerFont(textures->texture0yellow, glm::vec2(screenWidth - UIsizeX * numofFont - minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	minusX -= UIsizeX;
	// .
	TimerFont* point = new TimerFont(textures->texturePointyellow, glm::vec2(screenWidth - UIsizeX * numofFont - minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	timeFonts.push_back(point);
	minusX -= UIsizeX;
	//数字
	tenthsplaceNum = new TimerFont(textures->texture0yellow, glm::vec2(screenWidth - UIsizeX * numofFont - minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	minusX -= UIsizeX;
	hundredthsplaceNum = new TimerFont(textures->texture0yellow, glm::vec2(screenWidth - UIsizeX * numofFont - minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	minusX -= UIsizeX;
	//s
	TimerFont* S = new TimerFont(textures->textureSyellow, glm::vec2(screenWidth - UIsizeX * numofFont - minusX + offsetX, offsetY), glm::vec2(UIsizeX, UIsizeY));
	timeFonts.push_back(S);
}

void UIManager::ChangeTimerNumTexture(int num, TimerFont* timeNum)
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

void UIManager::UpdateTimerNum(float time)
{
	int hundredsplace = static_cast<int>(time / 100);
	int tensplace = static_cast<int>(time / 10) % 10;
	int onesplace = static_cast<int>(time) % 10;
	int tenthsplace = static_cast<int>(time * 10) % 10;
	int hundredthsplace = static_cast<int>(time * 100) % 10;
	ChangeTimerNumTexture(hundredsplace, hundredsplaceNum);
	ChangeTimerNumTexture(tensplace, tensplaceNum);
	ChangeTimerNumTexture(onesplace, onesplaceNum);
	ChangeTimerNumTexture(tenthsplace, tenthsplaceNum);
	ChangeTimerNumTexture(hundredthsplace, hundredthsplaceNum);
}

void UIManager::RenderTimer(float time)
{
	for (int i = 0; i < timeFonts.size(); i++)
	{
		timeFonts[i]->Render(*shader, orthoProjection);
	}
	if (time >= 100.0f)
	{
		hundredsplaceNum->Render(*shader, orthoProjection);
	}
	if (time >= 10.0f)
	{
		tensplaceNum->Render(*shader, orthoProjection);
	}
	onesplaceNum->Render(*shader, orthoProjection);
	tenthsplaceNum->Render(*shader, orthoProjection);
	hundredthsplaceNum->Render(*shader, orthoProjection);
}

void UIManager::InitGameOverText()
{
	glm::vec2 UISize = glm::vec2(800, 600);
	float screenOffsetX = 200.0f;
	float screenOffsetY = 400.0f;
	gameoverText = new Texture("res/texture/gamescene/gameovertext.png");
	gameoverTextUI = new UI(gameoverText, glm::vec2(screenWidth - UISize.x - screenOffsetX, screenHeight - UISize.y - screenOffsetY), UISize);
}

void UIManager::LoadTexture()
{
	redHeartTexture = new Texture("res/texture/playerUI/redheart.png");
	grayHeartTexture = new Texture("res/texture/playerUI/grayheart.png");

	jumpUITexture = new Texture("res/texture/playerUI/jump.png");
	nojumpUITexture = new Texture("res/texture/playerUI/nojump.png");
	dashUITexture = new Texture("res/texture/playerUI/dash.png");
	nodashUITexture = new Texture("res/texture/playerUI/nodash.png");
	rideUITexture = new Texture("res/texture/playerUI/ride.png");
	norideUITexture = new Texture("res/texture/playerUI/noride.png");

	aKeyTexture = new Texture("res/texture/playerUI/akey.png");
	dKeyTexture = new Texture("res/texture/playerUI/dkey.png");
	spaceKeyTexture = new Texture("res/texture/playerUI/spacekey.png");
	rightClickTexture = new Texture("res/texture/playerUI/rightclick.png");
}
