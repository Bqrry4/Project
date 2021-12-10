#pragma once
#include "NPC.h"
#include "ProjectileObject.h"

class LongRangeNPC : public NPC
{
	ProjectileObject projectile;
public:
	void Atack();
	void Update();
};

