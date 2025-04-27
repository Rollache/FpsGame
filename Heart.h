#pragma once
#include"UI.h"

class Heart :public UI
{
public:
	Heart(Texture* texture, glm::vec2 pos, glm::vec2 size) :UI(texture, pos, size)
	{

	}
	~Heart() = default;
};