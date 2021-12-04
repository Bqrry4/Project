#pragma once
#include "Entity.h"
#include "AnimatedObj.h"
#include "RigidBody.h"

enum class PlayerState {
	Repaos,
	Run,
	Jump,
	Atack,
	Ability
};

class Player : public AnimatedObj , public Entity
{
	float vx;
	float vy;

public: ///Masa redact, speed
	Player() : AnimatedObj(), vx(0), vy(0)
	{
		HP = 100;
		AP = 50;

	}
	//Player(Uint16 type, Hitbox hitbox, Uint8 row = 0, Uint8 column = 0, int AnimSpeed = 50, SDL_RendererFlip flip = SDL_FLIP_NONE) : AnimatedObj(type, hitbox, row, column, AnimSpeed, flip)
	//{
	//	RB = new RigidBody(mass);
	//	State = PlayerState::Repaos;
	//	vy = 0;
	//}
	~Player()
	{}
	

	void Update();
	virtual void Movement();
	virtual void Atack();
	void Delete()
	{

	}

	//void Jump();
	//void Draw();
};
