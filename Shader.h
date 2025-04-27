#pragma once
#include "base.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace glm;
using namespace std;

class Shader
{

public:
	Shader(string vertexPath, string fragmentPath);
	Shader(Shader* shader);

	~Shader();

	GLuint GetProgram()
	{
		return program;
	}

	void Bind()
	{
		glUseProgram(program);
	}

	void Unbind()
	{
		glUseProgram(0);
	}

	void SetBool(const char* name, bool value)
	{
		glUniform1i(GetLocation(name), (int)value);
	}

	void SetInt(const char* name, int val)
	{
		glUniform1i(GetLocation(name), val);
	}

	void SetFloat(const char* name, float val)
	{
		glUniform1f(GetLocation(name), val);
	}

	void SetVec3(const char* name, glm::vec3 val)
	{
		glUniform3fv(GetLocation(name), 1, &val[0]);
	}

	void SetVec4(const char* name, glm::vec4 val)
	{
		glUniform4fv(GetLocation(name), 1, &val[0]);
	}

	void SetMat3(const char* name, glm::mat3 val)
	{
		glUniformMatrix3fv(GetLocation(name), 1, GL_FALSE, &val[0][0]);
	}

	void SetMat4(const char* name, glm::mat4 val)
	{
		glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, &val[0][0]);
	}
private:
	//シェーダーまたはプログラムのコンパイルエラーをチェック
	void CheckCompileErrors(GLuint shader, string type);
	//シェーダープログラム内のユニフォーム変数の位置を取得
	GLuint GetLocation(const char* name);

private:
	GLuint program;
};

