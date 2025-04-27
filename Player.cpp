
#include"player.h"

Player::Player(glm::vec2 windowSize, GLFWwindow* window, Sound& sound)
{
	this->window = window;
	this->windowSize = windowSize;
	this->sound = sound;
	this->bowModel = glm::mat4(1.0);
	//プレーヤーステータス初期化
	ResetPlayerState();
	//Camera初期化
	camera.SetWindow(window);
	camera.UpdateCamera(isShooting);
	camera.SetPosition(position);

	view = camera.GetViewMatrix();
	projection = perspective(radians(camera.GetZoom()), windowSize.x / windowSize.y, 0.03f, 30000.0f);

	arrows = new ArrowManager();
	LoadModel();
	LoadTexture();
	LoadShader();
	InitCrosshair();
}

Player::~Player()
{
	for (Texture* crosshairTexture : crosshairTextures)
	{
		delete crosshairTexture;
	}
	delete crosshairShader;
	delete crosshairEffect;
	delete bow;
	delete bowShooting;
	delete bowShader;
	delete bowTexture;
	delete arrows;
}

void Player::Reset()
{
	ResetPlayerState();
	camera.Reset();
	camera.SetPosition(position);
	arrows->Reset();
	crosshairEffect->ResetTexture();
}

void Player::Update(float deltaTime, MonsterManager& monsters)
{
	MouseMovement(deltaTime);
	KeyboardInput(deltaTime);

	view = camera.GetViewMatrix();
	projection = perspective(radians(camera.GetZoom()), windowSize.x / windowSize.y, 0.03f, 30000.0f);
	//弓の更新
	BowUpdate(deltaTime);
	//矢の更新
	arrows->Update(deltaTime, camera, monsters, isHitTarget);
}

void Player::Render()
{
	if (isHolding)
	{
		RenderShootingBow();
	}
	else
	{
		RenderBow();
	}
	arrows->Render(projection, view);
	crosshairEffect->Render(camera.GetViewMatrix(), projection, camera);
}

void Player::UpdatePosition(float wallVelocity, float deltaTime)
{
	//壁と一緒に移動する
	if (isCollsionWithMoveWall == true)
	{
		nextPosition.z += wallVelocity * deltaTime;
	}
	//外に出ないよう
	float wallCollisionPosX = 264.0f;
	float wallCollisionPosY = 264.0f;
	float outOfMapPosZ = -10900.0f;
	//左と右方向
	if (position.x + nextPosition.x > wallCollisionPosX)
	{
		nextPosition.x = wallCollisionPosX - position.x;
	}
	if (position.x + nextPosition.x < -wallCollisionPosX)
	{
		nextPosition.x = -wallCollisionPosX - position.x;
	}
	//上方向
	if (position.y + nextPosition.y > wallCollisionPosY)
	{
		nextPosition.y = wallCollisionPosY - position.y;
	}
	//前方向
	if (position.z + nextPosition.z < outOfMapPosZ)
	{
		nextPosition.z = outOfMapPosZ - position.z;
	}
	//計算済みのポジションを実装
	position += nextPosition;
	leftCollision += nextPosition;
	rightCollision += nextPosition;
	topCollision += nextPosition;
	bottomCollision += nextPosition;
	camera.SetPosition(position);
}

void Player::UpdateCrosshair()
{
	float intervalFromCamera = 25.0f;
	glm::vec3 crosshairPos = camera.GetPosition() + (camera.GetFrontWithoutRecoilZ() * intervalFromCamera);
	crosshairEffect->SetPos(crosshairPos);
}

void Player::InitStartAnimation()
{
	position = glm::vec3(0.0f, -130.0f, -10200.0f);
	camera.Reset();
	camera.SetPosition(position);
}

void Player::LoadModel()
{
	bowShooting = new Model("res/model/bow_shooting.obj");
	bow = new Model("res/model/bow.obj");
}

void Player::LoadTexture()
{
	bowTexture = new Texture("res/texture/gamescene/BOW.png");
}

void Player::LoadShader()
{
	bowShader = new Shader("res/shader/bow.vert", "res/shader/bow.frag");
	bowShader->Bind();
	bowShader->SetInt("diffuse", 0);
	bowShader->Unbind();
}

void Player::RenderBow()
{
	bowShader->Bind();
	bowShader->SetMat4("projection", projection);
	bowShader->SetMat4("view", view);
	bowShader->SetMat4("model", bowModel);
	//テクスチャ
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bowTexture->GetId());
	//描画
	glBindVertexArray(bow->GetVAO());
	glDrawElements(GL_TRIANGLES, static_cast<GLuint>(bow->GetIndices().size()), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	bowShader->Unbind();
}

void Player::RenderShootingBow()
{
	// 透明の部分も正しく描画できるよう
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	bowShader->Bind();
	bowShader->SetMat4("projection", projection);
	bowShader->SetMat4("view", view);
	bowShader->SetMat4("model", bowModel);
	//弓のテクスチャ
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bowTexture->GetId());
	//描画
	glBindVertexArray(bowShooting->GetVAO());
	glDrawElements(GL_TRIANGLES, static_cast<GLuint>(bowShooting->GetIndices().size()), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glDisable(GL_BLEND);

	bowShader->Unbind();
}

void Player::MouseMovement(float deltaTime)
{
	double newMouseX, newMouseY;
	//現在のマウス座標
	glfwGetCursorPos(window, &newMouseX, &newMouseY);

	//マウスの初期値を保存
	if (firstMouse)
	{
		mouseX = newMouseX;
		mouseY = newMouseY;
		firstMouse = false;
	}
	//カメラ設置
	camera.yaw += ((newMouseX - mouseX) * mouseSensitivity);
	camera.pitch += ((mouseY - newMouseY) * mouseSensitivity);

	mouseX = newMouseX;
	mouseY = newMouseY;
	//カメラ回転防止
	if (camera.pitch > 89.0f)
	{
		camera.pitch = 89.0f;
	}
	if (camera.pitch < -89.0f)
	{
		camera.pitch = -89.0f;
	}

	camera.UpdateCamera(isShooting, deltaTime);
}

void Player::KeyboardInput(float deltaTime)
{
	gravity -= PLAYERGRAVITY;

	float velocity = movementSpeed * deltaTime;

	glm::vec3 forward = camera.GetForward();
	nextPosition = glm::vec3(0, 0, 0);

	bool isBesideWall = (isBesideLeftWall || isBesideRightWall);
	if (isDashing == false)
	{
		//WASD移動
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			nextPosition += forward * velocity;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			nextPosition -= forward * velocity;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			nextPosition -= camera.GetRight() * velocity;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			nextPosition += camera.GetRight() * velocity;
		}

		//走る
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS && !isDashing)
		{
			movementSpeed = RUNSPEED;
		}
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE && !isDashing)
		{
			movementSpeed = SPEED;
		}
	}

	//ダッシュ
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && isDashed == false && isDashing == false && !isQuickDroping)
	{
		sound.Play(DASH);
		isDashing = true;
		dashTime = DASHTIME;
		movementSpeed = DASHSPEED;

		//ジャンプできる
		if (isDoubleJumped == true)
		{
			isExtraJumped = false;
		}
		//壁移動できる
		isWallRiding = false;
		isWallRided = false;
	}
	if (isDashing)
	{
		dashTime -= deltaTime;
		gravity = 0;
		nextPosition += forward * velocity;
		if (dashTime <= 0)
		{
			dashTime = 0;
			isDashing = false;
			isDashed = true;
		}
	}
	//敵倒したら壁移動とダッシュをリセット
	if (isHitTarget == true)
	{
		isWallRided = false;
		isDashed = false;
		isHitTarget = false;
	}
	//extrajump
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isExtraJumped && !spaceKeyPressed && !isDashing && !isQuickDroping)
	{
		sound.Play(JUMP);
		if (gravity < 0)
		{
			gravity = 0;
		}
		gravity += JUMPSTRENGTH;
		//ちょっと壁蹴り
		if (isBesideLeftWall)
		{
			wallBounce = WALLBOUNCESTRENGTH;
		}
		else if (isBesideRightWall)
		{
			wallBounce = -WALLBOUNCESTRENGTH;
		}
		spaceKeyPressed = true;
		isExtraJumped = true;
		isWallRiding = false;
	}

	//壁移動
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && isBesideWall && !isWallRided && !isDashing && !isQuickDroping)
	{
		if (!isWallRiding)
		{
			sound.Play(TAKEWALL);
		}
		gravity = 0;
		movementSpeed = RIDESPEED;
		isWallRiding = true;
		spaceKeyPressed = true;
		isExtraJumped = false;
	}

	//二段ジャンプ
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && isJumped && !isDoubleJumped && !isBesideWall && !spaceKeyPressed && !isDashing && !isQuickDroping)
	{
		sound.Play(JUMP);
		if (gravity < 0)
		{
			gravity = 0;
		}
		gravity += JUMPSTRENGTH;
		spaceKeyPressed = true;
		isDoubleJumped = true;
	}

	//ジャンプ
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && !isJumped && !isDoubleJumped && !isBesideWall && !spaceKeyPressed && !isDashing && !isQuickDroping)
	{
		sound.Play(JUMP);

		if (gravity < 0)
		{
			gravity = 0;
		}
		gravity += JUMPSTRENGTH;
		isJumped = true;
		spaceKeyPressed = true;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		spaceKeyPressed = false;
		if (isWallRiding == true)
		{
			isWallRiding = false;
			isWallRided = true;
		}
	}
	//急速落下
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		eKeyPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_RELEASE && eKeyPressed == true)
	{
		isQuickDroping = true;
		eKeyPressed = false;
	}
	if (isQuickDroping)
	{
		gravity = -QUICKDROPSPEED;
	}
	//次の位置を計算
	nextPosition.y += gravity * deltaTime * 10;

	if (wallBounce != 0)
	{
		nextPosition.x += wallBounce * deltaTime;
		wallBounce = 0;
	}
}

void Player::ResetPlayerState()
{
	this->bowRecoil = 10.0f;
	mouseSensitivity = 0.05f;
	position = glm::vec3(0.0f, HEIGHT - 100, 0.0f);
	bottomCollision = position + glm::vec3(0, -HEIGHT, 0);
	topCollision = position;
	leftCollision = position - glm::vec3(WIDTH / 2, 0, 0);
	rightCollision = position + glm::vec3(WIDTH / 2, 0, 0);
	hp = 3;
	firstMouse = true;
	eKeyPressed = false;
	spaceKeyPressed = false;
	isJumped = false;
	isDoubleJumped = false;
	isExtraJumped = true;
	isBesideLeftWall = false;
	isBesideRightWall = false;
	isWallRiding = false;
	isWallRided = false;
	isQuickDroping = false;
	gravity = -PLAYERGRAVITY;
	wallBounce = 0.0f;
	movementSpeed = SPEED;

	isDashing = false;
	isDashed = false;
	dashTime = 0.0f;

	isHitTarget = false;
	isCollsionWithMoveWall = false;
	isCollsionWithExtraFloor = false;

	isShooting = false;
	isHolding = false;
	isAddedArrow = false;
}

void Player::InitCrosshair()
{
	//画像読み込み
	for (int i = 1; i <= 4; i++)
	{
		crosshairTextures.push_back(new Texture(("res/texture/crosshair/038" + std::to_string(i) + ".png").c_str()));
	}
	//shader読み込み
	crosshairShader = new Shader("res/shader/billboard.vert", "res/shader/billboard.frag");
	crosshairEffect = new Billboard(glm::vec3(0, 0, 0), 1.0f, crosshairTextures, crosshairShader);
	//動画の更新速度
	crosshairEffect->SetFrameDuration(0.2f);
}

void Player::BowUpdate(float deltaTime)
{
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		isShooting = true;
		crosshairEffect->UpdateToLaseFrame(deltaTime);

		if (!isHolding)
		{
			sound.Play(HOLD);
		}
		isHolding = true;
		if (isAddedArrow == false)
		{
			arrows->AddArrow();
			isAddedArrow = true;
		}
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE && isHolding == true)
	{
		isShooting = false;
		crosshairEffect->ResetTexture();

		isHolding = false;
		isAddedArrow = false;
		sound.Stop(HOLD);
		if (arrows->Shoot())
		{
			sound.Play(SHOOT);
		}
	}

	// 弓の位置を調整
	glm::vec3 bowPos = camera.GetPosition() + (camera.GetFront() * 0.3f) + (camera.GetUp() * -0.05f);
	bowModel = glm::mat4(1.0);
	bowModel[0] = glm::vec4(camera.GetRight(), 0.0);
	bowModel[1] = glm::vec4(camera.GetUp(), 0.0);
	bowModel[2] = glm::vec4(-camera.GetFront(), 0.0);
	bowModel[3] = glm::vec4(bowPos, 1.0);
	//弓を引いてるときの弓
	if (isHolding)
	{
		bowModel = translate(bowModel, glm::vec3(0.0f, 0.0f, -0.3f));
		bowModel = rotate(bowModel, radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
		bowModel = rotate(bowModel, radians(-45.0f), glm::vec3(1.0, 0.0, 0.0));
		bowModel = scale(bowModel, glm::vec3(0.030f));
		bowModel = translate(bowModel, glm::vec3(5.0f, 0.0f, 0.0f));
	}
	//一般状態
	else
	{
		bowModel = translate(bowModel, vec3(0.0f, 0.0f, -0.3f));
		bowModel = rotate(bowModel, radians(-120.0f), vec3(0.0, 1.0, 0.0));
		bowModel = rotate(bowModel, radians(-55.0f), vec3(1.0, 0.0, 1.0));
		bowModel = scale(bowModel, vec3(0.020f));
		bowModel = translate(bowModel, vec3(0.0f, 5.0f, -10.0f));
	}
}

