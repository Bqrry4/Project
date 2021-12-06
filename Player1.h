#pragma once
#include "Player.h"

class Player1 : public Player
{

public:
	Player1() : Player()
	{
		Key_Up_id = SDL_SCANCODE_W;
		Key_Down_id = SDL_SCANCODE_S;
		Key_Left_id = SDL_SCANCODE_A;
		Key_Right_id = SDL_SCANCODE_D;
	}

	//void Movement();
	//void Atack();
};

