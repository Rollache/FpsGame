#include "StartCube.h"

void StartCube::Render(Shader& shader)
{
	//MVP行列を計算
	glm::mat4 mvp = projection * view * modelMatrix;

	//シェーダーをバインドし、MVP行列を設定
	shader.Bind();
	shader.SetMat4("uMVP", mvp);

	//テクスチャをシェーダーにセット
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->GetId());
	shader.SetInt("uTexture", 0);

	//立方体を描画
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//シェーダーのバインドを解除
	shader.Unbind();
}
