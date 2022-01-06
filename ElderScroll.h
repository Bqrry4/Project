#pragma once
#include "GObject.h"

class ElderScroll : public GObject
{
	float vy;
	float middlePosition;
public:
	ElderScroll() : vy(10.0f), middlePosition(0.0f)
	{
		Interact = true;
	}
	void Update();
};

