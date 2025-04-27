#include"Arrow.h"

Arrow::Arrow(Shader* arrowShader, Model* arrowModel, Texture* arrowTexture)
{
	this->arrowModelMat = glm::mat4(1.0);
	this->arrowShader = arrowShader;
	this->arrowModel = arrowModel;
	this->arrowTexture = arrowTexture;
}

void Arrow::Render(glm::mat4 projection, glm::mat4 view)
{
	if (isAlive != false && status != idle)
	{
		arrowShader->Bind();

		arrowShader->SetMat4("projection", projection);
		arrowShader->SetMat4("view", view);
		arrowShader->SetMat4("model", arrowModelMat);
		//テクスチャをバインド
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, arrowTexture->GetId());
		//描画
		glBindVertexArray(arrowModel->GetVAO());
		glDrawElements(GL_TRIANGLES, static_cast<GLuint>(arrowModel->GetIndices().size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		arrowShader->Unbind();
	}
}

void Arrow::Update(float deltaTime, Camera& camera, MonsterManager& monsters, bool& isHitTarget)
{
	if (status == holding) //矢を引いている状態
	{
		holdTime += deltaTime;
		InitModel(camera);
		onHolding = true;
	}
	if (status == moving) //矢が発射された状態
	{
		//溜める時間が0.2秒以下なら無効
		if (holdTime <= 0.2f)
		{
			isAlive = false;
		}
		else
		{
			if (onHolding)
			{
				arrowModelMat[3] = glm::vec4(camera.GetPosition(), 1.0);
				startPosition = glm::vec3(arrowModelMat[3]);
				direction = normalize(camera.GetFront());
				arrowModelMat = scale(arrowModelMat, glm::vec3(2.0f));
				movement = direction * deltaTime * baseVelocity;
				onHolding = false;
				//RAYでモンスターとの衝突を判定
				CheckCollision(camera, monsters, isHitTarget);
			}
			glm::vec3 prevPosition = glm::vec3(arrowModelMat[3]);
			gravity -= ARROWGRAVITY * deltaTime;
			movement.y += gravity * deltaTime * 10;
			arrowModelMat[3] += glm::vec4(movement, 0.0f);

			//矢の距離が一定以上なら削除
			float distance = length(glm::vec3(arrowModelMat[3]) - startPosition);
			if (distance > MAXDISTANCE)
			{
				isAlive = false;
			}
		}
	}
}

void Arrow::InitModel(Camera& camera)
{
	glm::vec3 arrowPos = camera.GetPosition() + (camera.GetFront() * 0.3f) + (camera.GetUp() * -0.05f);
	arrowModelMat = glm::mat4(1.0);
	arrowModelMat[0] = glm::vec4(camera.GetRight(), 0.0);
	arrowModelMat[1] = glm::vec4(camera.GetUp(), 0.0);
	arrowModelMat[2] = glm::vec4(-camera.GetFront(), 0.0);
	arrowModelMat[3] = glm::vec4(arrowPos, 1.0);
	//モデル調整
	arrowModelMat = translate(arrowModelMat, glm::vec3(0.0f, 0.0f, -0.3f));
	arrowModelMat = rotate(arrowModelMat, radians(-180.0f), glm::vec3(0.0, 1.0, 0.0));
	arrowModelMat = rotate(arrowModelMat, radians(80.0f), glm::vec3(0.0, 0.0, 1.0));
	arrowModelMat = scale(arrowModelMat, glm::vec3(0.015f));
	arrowModelMat = translate(arrowModelMat, glm::vec3(2.0f, 0.0f, -10.0f));
}

//球体との当たり判定
bool Arrow::CheckSphereCollision(glm::vec3 start, glm::vec3 direction, glm::vec3 sphereCenter, float sphereRadius)
{
	glm::vec3 L = start - sphereCenter;
	float a = dot(direction, direction);
	float b = 2.0f * dot(direction, L);
	float c = dot(L, L) - sphereRadius * sphereRadius;
	float discriminant = b * b - 4.0f * a * c;

	if (discriminant < 0.0f)
	{
		return false;
	}

	float t0 = (-b - sqrt(discriminant)) / (2.0f * a);
	float t1 = (-b + sqrt(discriminant)) / (2.0f * a);

	if (t0 >= 0.0f || t1 >= 0.0f)
	{
		return true;
	}
	return false;
}

//AABB当たり判定
bool Arrow::CheckAABBCollision(glm::vec3 start, glm::vec3 direction, glm::vec3 minBound, glm::vec3 maxBound)
{
	float min = (minBound.x - start.x) / direction.x;
	float max = (maxBound.x - start.x) / direction.x;

	if (min > max)
	{
		std::swap(min, max);
	}

	float yMin = (minBound.y - start.y) / direction.y;
	float yMax = (maxBound.y - start.y) / direction.y;

	if (yMin > yMax)
	{
		std::swap(yMin, yMax);
	}
	if ((min > yMax) || (yMin > max))
	{
		return false;
	}

	if (yMin > min)
	{
		min = yMin;
	}
	if (yMax < max)
	{
		max = yMax;
	}


	float zMin = (minBound.z - start.z) / direction.z;
	float zMax = (maxBound.z - start.z) / direction.z;

	if (zMin > zMax)
	{
		std::swap(zMin, zMax);
	}
	if ((min > zMax) || (zMin > max))
	{
		return false;
	}


	if (zMin > min)
	{
		min = zMin;
	}
	if (zMax < max)
	{
		max = zMax;
	}


	return max >= 0.0f;
}

void Arrow::CheckCollision(Camera& camera, MonsterManager& monsterManager, bool& isHitTarget)
{
	for (int i = 0; i < monsterManager.monsters.size(); i++)
	{
		if (CheckAABBCollision(camera.GetPosition(), direction, monsterManager.monsters[i]->GetMinBound(), monsterManager.monsters[i]->GetMaxBound())
			&& isAlive == true)
		{
			monsterManager.monsters[i]->alive = false;
			isHitTarget = true;
			isAlive = false;
		}
	}
}