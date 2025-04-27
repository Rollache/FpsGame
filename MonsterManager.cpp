#include"MonsterManager.h"


MonsterManager::MonsterManager(glm::vec2 windowSize, Sound& sound)
{
	this->windowSize = windowSize;
	this->sound = sound;
	//アセット読み込み
	LoadModel();
	LoadTexture();
	LoadShader();
	//モンスター初期化
	InitMonsters();
	InitDeathTextures();
}

MonsterManager::~MonsterManager()
{
	for (Monster* monster : monsters)
	{
		delete monster;
	}
	monsters.clear();
	for (Billboard* effect : deathEffects)
	{
		delete effect;
	}
	deathEffects.clear();
	for (Texture* texture : deathTextures)
	{
		delete texture;
	}
	deathTextures.clear();

	delete deathEffectShader;
	delete normalMonsterModel;
	delete trackingMonsterModel;
	delete birdMonsterModel;
	delete monsterShader;
	delete normalMonsterTexture;
	delete trackingMonsterTexture;
	delete birdMonsterTexture;
}

void MonsterManager::Reset()
{
	for (auto monster : monsters)
	{
		delete monster;
	}
	monsters.clear();
	for (auto effect : deathEffects)
	{
		delete effect;
	}
	deathEffects.clear();

	addMonsterTime = 0.0f;

	InitMonsters();
}

void MonsterManager::InitMonsters()
{
	//-2000 から -10000 まで 800 ごとに一匹生成する
	glm::vec3 monsterPos = glm::vec3(0, 0, -2000);
	float interval = 800;
	for (int i = 0; i < 10; i++)
	{
		AddMonster(monsterPos.z);
		monsterPos.z -= interval;
	}
}

void MonsterManager::InitDeathTextures()
{
	int numofDeathTextures = 49;
	for (int i = 1; i <= numofDeathTextures; i++)
	{
		deathTextures.push_back(new Texture(("res/texture/deatheffect/1_" + std::to_string(i) + ".png").c_str()));
	}
	deathEffectShader = new Shader("res/shader/billboard.vert", "res/shader/billboard.frag");
}


void MonsterManager::Update(float deltaTime, glm::vec3 playerPos)
{
	for (int i = 0; i < monsters.size();)
	{
		monsters[i]->Update(deltaTime, playerPos);
		//モンスターが倒されたら
		if (!monsters[i]->IsAlive())
		{
			if (!monsters[i]->hitPlayer)
			{
				sound.Play(BOOMSE);
			}
			//copyしないよう
			float size = 100.0f;
			Billboard* deathEffect = new Billboard(monsters[i]->GetPos(), size, deathTextures, deathEffectShader);
			deathEffects.push_back(deathEffect);
			delete monsters[i];
			monsters.erase(monsters.begin() + i);
		}
		//外まで移動したら消す
		else if (monsters[i]->GetPos().z > outOfMapPosZ)
		{
			delete monsters[i];
			monsters.erase(monsters.begin() + i);
		}
		else
		{
			i++;
		}
	}

	//一定時間を経過したらモンスターを一体追加
	if (addMonsterTime <= 0)
	{
		addMonsterTime = ADDMONSTERTIME;
		AddMonster();
	}
	else
	{
		addMonsterTime -= deltaTime;
	}

	//死亡動画の更新
	for (int i = 0; i < deathEffects.size(); )
	{
		deathEffects[i]->Update(deltaTime);
		if (deathEffects[i]->IsAnimationOver())
		{
			delete deathEffects[i];
			deathEffects.erase(deathEffects.begin() + i);
		}
		else
		{
			i++;
		}
	}

}

//モンスターたちとプレーヤーの当たり判定
bool MonsterManager::IsCollided(glm::vec3 playerPos)
{
	for (int i = 0; i < monsters.size(); i++)
	{
		if (monsters[i]->IsPlayerCollided(playerPos))
		{
			sound.Play(TAKEHIT);
			monsters[i]->alive = false;
			monsters[i]->hitPlayer = true;
			return true;
		}
	}
	return false;
}


void MonsterManager::Render(glm::mat4 view, glm::mat4 projection, const Camera& camera)
{
	for (int i = 0; i < monsters.size(); i++)
	{
		monsters[i]->Render(view, projection);
	}

	for (int i = 0; i < deathEffects.size(); i++)
	{
		deathEffects[i]->Render(view, projection, camera);
	}
}

void MonsterManager::AddMonster(float posZ)
{
	glm::vec3 monsterPos = glm::vec3(0, 0, posZ);
	monsterPos.x = rand() % 500 - 250;
	monsterPos.y = rand() % 500 - 250;
	//モンスターの種類はランダムにする
	int randNum = rand() % 5;
	if (randNum < 2)
	{
		NormalMonster* normalMonster = new NormalMonster(monsterPos, normalMonsterModel, monsterShader, normalMonsterTexture);
		monsters.push_back(normalMonster);
	}
	else if (randNum == 2)
	{
		BirdMonster* birdMonster = new BirdMonster(monsterPos, birdMonsterModel, monsterShader, birdMonsterTexture);
		monsters.push_back(birdMonster);
	}
	else if (randNum > 2)
	{
		TrackingMonster* trackingMonster = new TrackingMonster(monsterPos, trackingMonsterModel, monsterShader, trackingMonsterTexture);
		monsters.push_back(trackingMonster);
	}
}

void MonsterManager::LoadModel()
{
	normalMonsterModel = new Model("res/model/fish.fbx");
	trackingMonsterModel = new Model("res/model/fish2.fbx");
	birdMonsterModel = new Model("res/model/bird.fbx");;
}

void MonsterManager::LoadShader()
{
	monsterShader = new Shader("res/shader/star.vert", "res/shader/star.frag");
	monsterShader->Bind();
	monsterShader->SetInt("diffuse", 0);
	monsterShader->Unbind();
}

void MonsterManager::LoadTexture()
{
	normalMonsterTexture = new Texture("res/texture/gamescene/fish.png");
	trackingMonsterTexture = new Texture("res/texture/gamescene/fish2.png");
	birdMonsterTexture = new Texture("res/texture/gamescene/bird.png");
}