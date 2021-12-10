#pragma once
#include "NPC.h"

class ShortRangeNPC : public NPC
{
public:
	ShortRangeNPC() : NPC()
	{}


	void Atack();
	void Update();
};

