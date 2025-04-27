#include"shader.h"

Shader::Shader(string vertexPath, string fragmentPath)
{
	string vertexCode;
	string fragmentCode;
	//ファイル読み込み
	ifstream vShaderFile(vertexPath);
	ifstream fShaderFile(fragmentPath);
	stringstream vShaderStream, fShaderStream;

	string line;
	//頂点シェーダー内容を取得
	while (!vShaderFile.eof())
	{
		getline(vShaderFile, line);
		vShaderStream << line + '\n';
	}
	//ピクセルシェーダー内容を取得
	while (!fShaderFile.eof())
	{
		getline(fShaderFile, line);
		fShaderStream << line + '\n';
	}
	//文字列として格納
	vertexCode = vShaderStream.str();
	fragmentCode = fShaderStream.str();
	//C文字列へ変換
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	//頂点シェーダーを作成
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);
	CheckCompileErrors(vertexShader, "VERTEX");
	//ピクセルシェーダーを作成
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);
	CheckCompileErrors(fragmentShader, "FRAGMENT");
	//シェーダープログラムを作成
	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	CheckCompileErrors(program, "PROGRAM");
	//シェーダーオブジェクトを削除
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::Shader(Shader* shader)
{
	program = shader->GetProgram();
}

Shader::~Shader()
{
	glDeleteProgram(program);
}

void Shader::CheckCompileErrors(GLuint shader, string type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << endl;
		}
	}
}

GLuint Shader::GetLocation(const char* name)
{
	GLint location = glGetUniformLocation(program, name);

	return location;
}

