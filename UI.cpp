#include"UI.h"
#include <iostream>

UI::UI(Texture* texture, glm::vec2 pos, glm::vec2 size)
{
	this->texture = texture;
	this->position = pos;
	this->size = size;
	this->visible = true;
	Init();
	offsetY = 0.0f;
}

void UI::Render(Shader& shader, const glm::mat4& orthoProjection, float alpha)
{
	//シェーダープログラム
	glUseProgram(shader.GetProgram());
	//透明部分も表示できるよう
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader.Bind();
	shader.SetFloat("alpha", alpha);
	shader.SetMat4("projection", orthoProjection);
	shader.SetFloat("offsetY", offsetY);
	//テクスチャ
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetId());
	shader.SetInt("texture1", 0);
	//VAOをバインド
	glBindVertexArray(VAO);
	//描画
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	//バインドを解除
	glBindVertexArray(0);
	glDisable(GL_BLEND);
	shader.Unbind();
}

void UI::Init()
{
	float vertices[] = {
		position.x, position.y, 0.0f, 0.0f,                 // 左下
		position.x + size.x, position.y, 1.0f, 0.0f,       // 右下
		position.x + size.x, position.y + size.y, 1.0f, 1.0f, // 右上
		position.x, position.y + size.y, 0.0f, 1.0f        // 左上
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};
	//VAO VBO EBOの生成とバインド
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	//VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//位置
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//テクスチャ座標
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//バインドを解除
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
