#include"World.h"

World::World(GLFWwindow* window, glm::vec2 windowSize, CharacterTexture& characterTextures, Sound& sound)
{
	this->window = window;
	this->windowSize = windowSize;
	this->characterTextures = &characterTextures;
	this->sound = sound;
	//ゲームの初期化
	InitFramebuffer();
	InitScreenQuad();

	LoadTexture();
	LoadShader();
	InitSkyBox();
	InitExtraFloorManager();
	InitWallManager();
	InitStartCubeManager();

	player = new Player(windowSize, window, sound);
	monsters = new MonsterManager(windowSize, sound);
	star = new Star();
	signpostManager = new SignpostManager();
	uiManager = new UIManager(windowSize.x, windowSize.y, *player, characterTextures);
	timer = new Timer();

	isStartAnimationPlayed = false;
}
World::~World()
{
	delete timer;
	delete uiManager;
	delete signpostManager;
	delete star;
	delete player;
	delete monsters;
	delete wallManager;
	delete extraFloorManager;
	delete startCubeManager;

	delete extraFloorTexture;
	delete startCubeLRTexture;
	delete startCubeTopTexture;
	delete startCubeBottomTexture;
	delete startCubeBackTexture;
	delete wallTexture;

	delete screenShader;
}

void World::Reset()
{
	player->Reset();
	monsters->Reset();
	wallManager->Reset();
	uiManager->Reset(*player);
	timer->reset();
	star->Reset();
	isGameClear = false;
	isGameOver = false;
	currentTime = 0.0f;

	if (!isStartAnimationPlayed)
	{
		InitStartAnimation();
		pauseTime = 1.5f;
	}
}

void World::Update(float deltaTime)
{
	if (isStartAnimationPlayed)
	{
		GameUpdate(deltaTime);
	}
	else
	{
		StartAnimationUpdate(deltaTime);
	}
}

void World::StartAnimationUpdate(float deltaTime)
{
	star->Update(deltaTime, *player, isGameClear);
	signpostManager->Update(deltaTime);
	//カメラを星の位置からプレーヤー最初の位置まで移動
	if (pauseTime > 0.0f)
	{
		pauseTime -= deltaTime;
	}
	else
	{
		float animationTime = 3.0f;
		//アニメションでカメラの移動距離
		float cameraMoveOffsetY = 40.0f;
		float cameraMoveOffsetZ = 10200.0f;
		player->position.y += cameraMoveOffsetY * deltaTime / animationTime;
		player->position.z += cameraMoveOffsetZ * deltaTime / animationTime;
		player->camera.SetPosition(player->position);
	}
	//アニメションが終わればプレーヤー状態をリセットする
	if (player->position.z >= 0.0f)
	{
		isStartAnimationPlayed = true;
		player->Reset();
		timer->reset();
	}
}

void World::GameUpdate(float deltaTime)
{
	//経過時間を取得
	elapsedTime = timer->getElapsedSeconds();
	//プレーヤーの更新処理
	player->Update(deltaTime, *monsters);
	//モンスターの更新処理
	monsters->Update(deltaTime, player->GetPos());
	//動く壁の更新処理
	wallManager->Update(*player, deltaTime);
	//壁とプレーヤーの当たり判定
	wallManager->IsCollision(*player);
	//動かない床の当たり判定
	extraFloorManager->IsFloorCollision(*player);
	//プレーヤー位置を更新
	player->UpdatePosition(wallManager->GetVelocity(), deltaTime);
	//照準の更新
	player->UpdateCrosshair();
	//プレーヤーとモンスターの当たり判定
	if (monsters->IsCollided(player->GetPos()))
	{
		player->hp -= 1;
		uiManager->PlayerHurt(player->hp);
		player->camera.Shake();
	}
	//他のアイコン
	star->Update(deltaTime, *player, isGameClear);
	if (isGameClear)
	{
		currentTime = GetElapsedTime();
		sound.Play(GOAL);
	}
	signpostManager->Update(deltaTime);
	//iconとuiの更新処理
	uiManager->Update(deltaTime, *player, elapsedTime);
}

void World::Render()
{
	if (isStartAnimationPlayed)
	{
		GameRender();
	}
	else
	{
		StartAnimationRender();
	}
}

void World::StartAnimationRender()
{
	//スカイボックスの描画
	skybox.Render(player->GetView(), player->GetProjection());
	//動かない床の描画
	extraFloorManager->SetViewProjection(player->GetView(), player->GetProjection());
	extraFloorManager->Render();
	//動く壁の描画
	wallManager->SetViewProjection(player->GetView(), player->GetProjection());
	wallManager->Render();
	//周りの壁の描画
	startCubeManager->SetViewProjection(player->GetView(), player->GetProjection());
	startCubeManager->Render();
	//他のオブジェクトの描画
	star->Render(player->GetView(), player->GetProjection());
	signpostManager->Render(player->GetView(), player->GetProjection());
	//モンスターの描画
	monsters->Render(player->GetView(), player->GetProjection(), player->camera);
}

void World::GameRender()
{
	//スカイボックスの描画
	skybox.Render(player->GetView(), player->GetProjection());
	//動かない床の描画
	extraFloorManager->SetViewProjection(player->GetView(), player->GetProjection());
	extraFloorManager->Render();
	//動く壁の描画
	wallManager->SetViewProjection(player->GetView(), player->GetProjection());
	wallManager->Render();
	//周りの壁の描画
	startCubeManager->SetViewProjection(player->GetView(), player->GetProjection());
	startCubeManager->Render();
	//他のオブジェクトの描画
	star->Render(player->GetView(), player->GetProjection());
	signpostManager->Render(player->GetView(), player->GetProjection());
	//モンスターの描画
	monsters->Render(player->GetView(), player->GetProjection(), player->camera);
	//プレーヤーの描画
	player->Render();
	//uiとiconの描画
	uiManager->Render(elapsedTime);
}

void World::GameOverRender()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GameRender();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//ゲームオーバーしたら画面は赤くなる
	screenShader->Bind();
	screenShader->SetFloat("redIntensity", 0.5f);
	screenShader->SetInt("screenTexture", 0);

	glDisable(GL_DEPTH_TEST);

	glBindVertexArray(screenQuadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glEnable(GL_DEPTH_TEST);
	//gameoverのフォントを表示
	uiManager->RenderGameOverText();
}

void World::GameClearRender()
{
	skybox.Render(player->GetView(), player->GetProjection());
	star->Render(player->GetView(), player->GetProjection());
}

bool World::IsGameOver()
{
	//炎の位置
	float gameoverCollisionY = -290.0f;
	//後ろ魔法陣の位置
	float gameoverCollisionZ = 95.0f;
	if (player->GetPos().y < gameoverCollisionY || player->GetPos().z>gameoverCollisionZ || player->hp <= 0)
	{
		if (!isGameOver)
		{
			sound.Play(DEATH);
		}
		//移動やダッシュで外に出ないよう
		if (player->GetPos().z > gameoverCollisionZ + 5)
		{
			player->SetPos(glm::vec3(player->GetPos().x, player->GetPos().y, gameoverCollisionZ));
		}
		if (player->GetPos().y < gameoverCollisionY)
		{
			player->SetPos(glm::vec3(player->GetPos().x, gameoverCollisionY, player->GetPos().z));
		}
		player->camera.SetPosition(player->GetPos());

		player->hp = 0;
		//hp表示を更新
		uiManager->PlayerHurt(player->hp);

		isGameOver = true;
	}

	return isGameOver;
}

void World::PlayGameClearAnimation(float deltaTime)
{
	player->camera.yaw = 270.0f;
	player->camera.pitch = 30.0f;
	player->camera.UpdateCamera(false, deltaTime);
	star->UpdateGameClearAnimation(deltaTime);
}

//フレームバッファを初期化
void World::InitFramebuffer()
{
	//FBO生成
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	//テクスチャ
	glGenTextures(1, &screenTexture);
	glBindTexture(GL_TEXTURE_2D, screenTexture);
	//テクスチャのサイズとフォーマットを設定
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowSize.x, windowSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	//テクスチャの線形補間
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//フレームバッファにテクスチャをアタッチ
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);
	//RBOを生成
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowSize.x, windowSize.y);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	//バインドを解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void World::InitScreenQuad()
{
	float quadVertices[] = {
		// 位置     // テクスチャ座標
		-1.0f, -1.0f,  0.0f, 0.0f, // 左下
		 1.0f, -1.0f,  1.0f, 0.0f, // 右下
		 1.0f,  1.0f,  1.0f, 1.0f, // 右上
		-1.0f,  1.0f,  0.0f, 1.0f  // 左上
	};

	GLuint quadIndices[] = {
		0, 1, 2,
		2, 3, 0
	};

	//vaoの生成とバインド
	glGenVertexArrays(1, &screenQuadVAO);
	glGenBuffers(1, &screenQuadVBO);
	glBindVertexArray(screenQuadVAO);
	//vbo生成とバインド
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
	//ebo生成とバインド
	GLuint screenQuadEBO;
	glGenBuffers(1, &screenQuadEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screenQuadEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);
	//頂点の位置座標の設定
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//頂点テクスチャ座標の設定
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//バインド解除
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void World::InitSkyBox()
{
	std::vector<std::string> faces
	{
		"res/texture/skybox/right.png",
		"res/texture/skybox/left.png",
		"res/texture/skybox/top.png",
		"res/texture/skybox/bottom.png",
		"res/texture/skybox/front.png",
		"res/texture/skybox/back.png"
	};
	skybox.SetFaces(faces);
	skybox.Init();
}


void World::InitExtraFloorManager()
{
	extraFloorManager = new ExtraFloorManager(extraFloorTexture, sound);
	extraFloorManager->AddFloor();
	extraFloorManager->AddFloor(glm::vec3(0.0f, -150.0f, -10200.0f));
}

void World::InitWallManager()
{
	wallManager = new WallManager(wallTexture, sound);
	wallManager->InitWalls();
}
void World::InitStartCubeManager()
{
	startCubeManager = new StartCubeManager(startCubeLRTexture, startCubeTopTexture, startCubeBottomTexture, startCubeBackTexture);
	startCubeManager->InitStartCube();
}

void World::LoadTexture()
{
	wallTexture = new Texture("res/texture/gamescene/wall.png");

	extraFloorTexture = new Texture("res/texture/gamescene/ice.png");

	startCubeLRTexture = new Texture("res/texture/gamescene/startcube.png");
	startCubeTopTexture = new Texture("res/texture/gamescene/startcube.png");

	startCubeBottomTexture = new Texture("res/texture/gamescene/startcubebottom.png");
	startCubeBackTexture = new Texture("res/texture/gamescene/magiccircle.png");
}

void World::LoadShader()
{
	screenShader = new Shader("res/shader/screen.vert", "res/shader/screen.frag");
}

void World::InitStartAnimation()
{
	player->InitStartAnimation();
}
