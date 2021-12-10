#pragma once
#include "Entity.h"
#include "AnimatedObj.h"



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

protected:
	SDL_Scancode Key_Up_id;
	SDL_Scancode Key_Down_id;
	SDL_Scancode Key_Left_id;
	SDL_Scancode Key_Right_id;
	SDL_Scancode Key_Atack_id;
	SDL_Scancode Key_Ability_id;

public: 
	Player() : AnimatedObj(), vx(0), vy(0), Key_Up_id(SDL_SCANCODE_UP), Key_Down_id(SDL_SCANCODE_DOWN), Key_Left_id(SDL_SCANCODE_LEFT), Key_Right_id(SDL_SCANCODE_RIGHT), Key_Atack_id(SDL_SCANCODE_SPACE), Key_Ability_id(SDL_SCANCODE_Z)
	{
		ObjectClassId = 2;

		collide.Is = true;
		collide.WithOthers = true;
		HP = 100;
		AP = 50;
		AtRange = 13;

	}
	//Player(Uint16 type, Hitbox hitbox, Uint8 row = 0, Uint8 column = 0, int AnimSpeed = 50, SDL_RendererFlip flip = SDL_FLIP_NONE) : AnimatedObj(type, hitbox, row, column, AnimSpeed, flip)
	//{
	//	RB = new RigidBody(mass);
	//	State = PlayerState::Repaos;
	//	vy = 0;
	//}
	~Player()
	{}
	

	virtual void Update();
	virtual void Movement();
	virtual void Jump();
	//virtual void Atack();
	virtual void Ability();
	virtual void IsDiyng();
	void Delete()
	{

	}
	//void Jump();
	//void Draw();
};
