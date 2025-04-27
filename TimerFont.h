#pragma once
#include"UI.h"

class TimerFont :public UI
{
public:
	TimerFont(Texture* texture, glm::vec2 pos, glm::vec2 size) :UI(texture, pos, size)
	{
	}

	~TimerFont() = default;
};

