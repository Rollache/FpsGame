#include "Cube.h"

Cube::Cube(const glm::vec3& pos, const glm::vec3& scale, float rotationAngle, const glm::vec3& rotationAxis)
{
	this->position = pos;
	this->scale = scale;
	this->rotationAngle = rotationAngle;
	this->rotationAxis = rotationAxis;

	//モデル行列の初期化
	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::rotate(modelMatrix, rotationAngle, rotationAxis); //回転
	modelMatrix = glm::translate(modelMatrix, position); //位置
	modelMatrix = glm::scale(modelMatrix, scale); //スケール
	InitData();
}

void Cube::SetViewProjection(const glm::mat4& view, const glm::mat4& projection)
{
	this->view = view;
	this->projection = projection;
}

void Cube::Render(Shader& shader)
{
	glm::mat4 mvp = projection * view * modelMatrix; //MVP行列を計算

	shader.Bind();
	shader.SetMat4("uMVP", mvp); //シェーダーにMVPを送信

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0); //立方体を描画
	glBindVertexArray(0);

	shader.Unbind();
}

//AABB判定
bool Cube::CheckCollision(const Cube& other) const
{
	//立方体のAABBを取得
	std::pair<glm::vec3, glm::vec3> boxA = this->GetBoundingBoxWorld();
	glm::vec3 minA = boxA.first;
	glm::vec3 maxA = boxA.second;

	std::pair<glm::vec3, glm::vec3> boxB = other.GetBoundingBoxWorld();
	glm::vec3 minB = boxB.first;
	glm::vec3 maxB = boxB.second;

	//交差判定を行う
	if (maxA.x < minB.x || minA.x > maxB.x) return false; //X軸
	if (maxA.y < minB.y || minA.y > maxB.y) return false; //Y軸
	if (maxA.z < minB.z || minA.z > maxB.z) return false; //Z軸

	return true; //衝突している
}

//指定された点が立方体の内部にあるかを判定
bool Cube::IsPlayerInside(const glm::vec3& point) const
{
	//立方体のAABBを取得
	std::pair<glm::vec3, glm::vec3> box = GetBoundingBoxWorld();
	glm::vec3 minV = box.first;
	glm::vec3 maxV = box.second;

	//点がAABBの範囲内にあるか判定
	return (point.x >= minV.x && point.x <= maxV.x &&
		point.y >= minV.y && point.y <= maxV.y &&
		point.z >= minV.z && point.z <= maxV.z);
}

//点の移動経路を細かく分けて、立方体内部にあるかを判定
bool Cube::IsPlayerInsideWithSubsteps(const glm::vec3& start, const glm::vec3& end, int substeps) const
{
	std::pair<glm::vec3, glm::vec3> box = GetBoundingBoxWorld();
	glm::vec3 minV = box.first;
	glm::vec3 maxV = box.second;

	//開始地点がすでに立方体内にある場合
	if (start.x >= minV.x && start.x <= maxV.x &&
		start.y >= minV.y && start.y <= maxV.y &&
		start.z >= minV.z && start.z <= maxV.z)
	{
		return true;
	}

	//細かく分割して、各ステップで判定
	glm::vec3 step = (end - start) / static_cast<float>(substeps);
	glm::vec3 current = start;
	const float epsilon = 1e-5f; //誤差の考慮

	for (int i = 0; i <= substeps; ++i)
	{
		if (current.x >= minV.x - epsilon && current.x <= maxV.x + epsilon &&
			current.y >= minV.y - epsilon && current.y <= maxV.y + epsilon &&
			current.z >= minV.z - epsilon && current.z <= maxV.z + epsilon)
		{
			return true;
		}
		current += step;
	}

	return false;
}

void Cube::RotateAroundZ(float angleDegrees)
{
	//角度をラジアンに変換
	float angleRadians = glm::radians(angleDegrees);

	//回転角度と回転軸を更新
	rotationAngle += angleRadians;
	rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);

	//回転行列を適用
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), angleRadians, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrix = rotation * modelMatrix;

	//[0, 2π] の範囲内に収める
	if (rotationAngle > glm::two_pi<float>())
	{
		rotationAngle -= glm::two_pi<float>();
	}
	else if (rotationAngle < 0.0f)
	{
		rotationAngle += glm::two_pi<float>();
	}
}

std::pair<glm::vec3, glm::vec3> Cube::GetBoundingBoxWorld() const
{
	//立方体のローカル座標の8つの頂点
	const glm::vec3 localVerts[8] =
	{
		glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f,  0.5f, -0.5f), glm::vec3(-0.5f,  0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.5f, -0.5f,  0.5f),
		glm::vec3(0.5f,  0.5f,  0.5f), glm::vec3(-0.5f,  0.5f,  0.5f)
	};

	//ワールド座標に変換
	glm::vec3 worldVerts[8];
	for (int i = 0; i < 8; i++)
	{
		glm::vec4 tmp = modelMatrix * glm::vec4(localVerts[i], 1.0f);
		worldVerts[i] = glm::vec3(tmp);
	}

	//AABBの最小・最大座標を求める
	glm::vec3 minV = worldVerts[0];
	glm::vec3 maxV = worldVerts[0];
	for (int i = 1; i < 8; i++)
	{
		if (worldVerts[i].x < minV.x) minV.x = worldVerts[i].x;
		if (worldVerts[i].y < minV.y) minV.y = worldVerts[i].y;
		if (worldVerts[i].z < minV.z) minV.z = worldVerts[i].z;

		if (worldVerts[i].x > maxV.x) maxV.x = worldVerts[i].x;
		if (worldVerts[i].y > maxV.y) maxV.y = worldVerts[i].y;
		if (worldVerts[i].z > maxV.z) maxV.z = worldVerts[i].z;
	}

	return std::make_pair(minV, maxV);
}

void Cube::InitData()
{
	float vertices[] =
	{
		// 前 　　　　uv座標
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		//後ろ
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		//左
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		//右
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 //上
		 -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,
		  0.5f,  0.5f, -0.5f,  1.0f, 0.0f,
		  0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		 //下
		 -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		  0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		  0.5f, -0.5f,  0.5f,  1.0f, 1.0f,
		 -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,
	};

	unsigned int indices[] = {
		//前
		0, 1, 2,
		2, 3, 0,
		//後ろ
		4, 5, 6,
		6, 7, 4,
		//左
		8, 9, 10,
		10, 11, 8,
		//右
		12, 13, 14,
		14, 15, 12,
		//上
		16, 17, 18,
		18, 19, 16,
		//下
		20, 21, 22,
		22, 23, 20
	};

	// VAOを生成
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// VAOをバインド
	glBindVertexArray(VAO);

	// VBOをバインドしてデータを設定
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// EBOをバインドしてデータを設定
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 頂点属性を設定
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 頂点属性を設定
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// VAOのバインドを解除
	glBindVertexArray(0);
}