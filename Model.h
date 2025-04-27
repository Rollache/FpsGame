#pragma once
#include "base.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;
using namespace Assimp;

class Model
{
public:
	Model(const string& path);

	GLuint GetVAO()
	{
		return VAO;
	}

	vector<GLuint> GetIndices()
	{
		return indices;
	}
private:
	void LoadModel(const string& path);
	//ノードの処理：ノード内のすべてのメッシュを処理し、子ノードを再帰的に処理
	void ProcessNode(aiNode* node, const aiScene* scene);
	//メッシュの処理：頂点データ、法線、テクスチャ座標を抽出し、インデックスを保存
	void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	//VAO VBO EBOの生成とバインド
	void Init();

private:
	vector<GLfloat> vertices;
	vector<GLuint> indices;
	GLuint VAO;
};
