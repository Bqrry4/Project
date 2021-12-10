#pragma once
#include "Player.h"

class Swordsman : public Player
{
public:
	Swordsman() : Player()
	{}
	void Atack();
	void Update();
};

