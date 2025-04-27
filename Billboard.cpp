#include "Billboard.h"

Billboard::Billboard(glm::vec3 position, float size, std::vector<Texture*> textures, Shader* shader)
{
	this->position = position;
	this->size = size;
	this->textures = textures;
	this->shader = shader;

	currentFrame = 0; //現在のフレーム
	elapsedTime = 0.0f; //経過時間
	frameDuration = 0.02f; //フレームの表示時間
	alpha = 1.0f; //アルファ値
	isOver = false; //アニメーションが終了したかのフラグ
	InitQuad(); //四角形の初期化
}

Billboard::~Billboard()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Billboard::Update(float deltaTime)
{
	elapsedTime += deltaTime; //経過時間を加算

	//一定時間が経過したら次のフレームへ
	if (elapsedTime >= frameDuration)
	{
		elapsedTime = 0.0f; //経過時間をリセット
		currentFrame++; //フレームを進める

		//最後のフレームに達した場合、アニメーション終了フラグを立てる
		if (currentFrame >= textures.size())
		{
			isOver = true;
		}
	}
}

void Billboard::Render(glm::mat4 view, glm::mat4 projection, const Camera& camera)
{
	//アニメーションが終了していたら描画しない
	if (isOver) return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader->Bind(); //シェーダーをバインド

	glBindVertexArray(VAO);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, position); //Billboard の位置を適用

	//Billboard は常にカメラの方向を向くようにする
	model[0] = glm::vec4(camera.GetRight(), 0.0); //X 軸方向
	model[1] = glm::vec4(camera.GetUp(), 0.0); //Y 軸方向
	model[2] = glm::vec4(-camera.GetFront(), 0.0); //Z 軸方向（カメラに向くよう反転）

	model = glm::scale(model, glm::vec3(size)); //サイズを適用

	shader->SetMat4("model", model);
	shader->SetMat4("view", view);
	shader->SetMat4("projection", projection);
	shader->SetFloat("alpha", 1.0f);

	//テクスチャをバインド
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[currentFrame]->GetId());
	shader->SetInt("texture1", 0);

	//四角形を描画
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glDisable(GL_BLEND);

	shader->Unbind(); //シェーダーのバインドを解除
}

//最後のフレームまでアニメーションを進める
void Billboard::UpdateToLaseFrame(float deltaTime)
{
	elapsedTime += deltaTime;

	//フレームを進める
	if (elapsedTime >= frameDuration)
	{
		elapsedTime = 0.0f;
		currentFrame++;

		//最後のフレームで止める
		if (currentFrame >= textures.size())
		{
			currentFrame = textures.size() - 1;
		}
	}
}

//Billboard のテクスチャをリセット
void Billboard::ResetTexture()
{
	currentFrame = 0;
}

//四角形の頂点データを初期化
void Billboard::InitQuad()
{
	//頂点データ
	float quadVertices[] =
	{
		//位置             //テクスチャUV座標
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, //左下
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, //右下
		 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, //右上
		-0.5f,  0.5f, 0.0f,  0.0f, 1.0f  //左上
	};

	//インデックスデータ
	GLuint indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	//VAO VBO EBOを生成
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	//VAOをバインド
	glBindVertexArray(VAO);

	//VBOをバインドしてデータを転送
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	//EBOをバインドしてデータを転送
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//頂点属性の設定 位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//頂点属性の設定 UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//バインド解除
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
