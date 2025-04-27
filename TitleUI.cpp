
#include "TitleUI.h"

TitleUI::TitleUI(float screenWidth, float screenHeight, CharacterTexture& textures)
{
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
	this->textures = &textures;
	shader = new Shader("res/shader/UI.vert", "res/shader/UI.frag");
	orthoProjection = glm::ortho(0.0f, screenWidth, screenHeight, 0.0f);
	textAlpha = 1.0f;
	alphaUpdateSpeed = -0.01f;
	Init();
}

TitleUI::~TitleUI()
{
	delete shader;
	delete bgTexture;
	delete textTexture;
	delete nameTexture;
	delete bg;
	delete text;
	delete name;
}

void TitleUI::Update(float deltaTime)
{
	//出たり消えたりの処理
	textAlpha += alphaUpdateSpeed;
	if (textAlpha >= 1.0f || textAlpha < -0.0f)
	{
		alphaUpdateSpeed = -alphaUpdateSpeed;
	}
}

void TitleUI::render()
{
	shader->Bind();
	bg->Render(*shader, orthoProjection);
	glDisable(GL_DEPTH_TEST);
	text->Render(*shader, orthoProjection, textAlpha);
	name->Render(*shader, orthoProjection);
	shader->Unbind();
}

void TitleUI::Init()
{
	bgTexture = new Texture("res/texture/titlescene/bg.jpg");
	bg = new UI(bgTexture, glm::vec2(0, 0), glm::vec2(screenWidth, screenHeight));

	glm::vec2 textTextureSize = glm::vec2(800, 600);
	float xOffset = -400;
	float yOffset = -450;

	textTexture = new Texture("res/texture/titlescene/titleText.png");
	text = new UI(textTexture, glm::vec2(screenWidth / 2 + xOffset, screenHeight + yOffset), textTextureSize);

	glm::vec2 nameTextureSize = glm::vec2(1200, 720);
	xOffset = -600;
	yOffset = -200;

	nameTexture = new Texture("res/texture/titlescene/name.png");
	name = new UI(nameTexture, glm::vec2(screenWidth / 2 + xOffset, yOffset), nameTextureSize);
}