#include"texture.h"


Texture::Texture(const char* path)
{
	//一つのテクスチャオブジェクトを生成
	glGenTextures(1, &id);

	//画像の幅、高さ、カラーチャンネル数を格納する
	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		int format;
		if (nrComponents == 1 || nrComponents == 2)
		{
			format = GL_RED;
		}
		else if (nrComponents == 3)
		{
			format = GL_RGB;
		}
		else if (nrComponents == 4)
		{
			format = GL_RGBA;
		}
		//生成したテクスチャIDをバインド
		glBindTexture(GL_TEXTURE_2D, id);
		//テクスチャ画像をGPUに転送
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		//ミップマップを生成
		glGenerateMipmap(GL_TEXTURE_2D);
		//テクスチャのピクセル配置のアライメントを1に設定
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		//テクスチャのラッピングを設定
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//テクスチャの拡大・縮小時の補間方法を設定
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//画像データを開放
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
}
