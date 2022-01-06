#pragma once
#include "GObject.h"
class FixedObject : public GObject
{
public:
	FixedObject()
	{
		Interact = true;
		collide.WithOthers = true;
	}
	~FixedObject() = default;
	void Update() {}
};

