#pragma once
#include "base.h"
#include <iostream>

class Texture
{
private:
	GLuint id;
public:
	Texture(const char* path);

	GLuint GetId()
	{
		return id;
	}
};
