#pragma once
#include "Player.h"
#include "ProjectileObject.h"

class Archer : public Player
{
	ProjectileObject arrow;
public:
	Archer() : Player()
	{}
	bool Parse(XMLElement* root, int iObject);
	void Atack();
	void Update();
};

