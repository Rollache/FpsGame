#pragma once
#include"UI.h"

class Action :public UI
{
public:
	Action(Texture* texture, glm::vec2 pos, glm::vec2 size) :UI(texture, pos, size)
	{

	}

	Action() = default;

};

