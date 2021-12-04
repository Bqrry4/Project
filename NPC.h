#pragma once
#include "AnimatedObj.h"
#include "Entity.h"

class NPC : public AnimatedObj, public Entity
{

public:
	NPC()
	{
		HP = rand() % 20 + 100;
	}

	virtual void Movement();
	virtual void Atack();

};

