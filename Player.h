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

	static float a;
	static float xSpeed;
	static float ySpeed;
	static float Gravity;

	SDL_Point CheckPointPosition;
	float AbilityDT;

protected:
	SDL_Scancode Key_Up_id;
	SDL_Scancode Key_Down_id;
	SDL_Scancode Key_Left_id;
	SDL_Scancode Key_Right_id;
	SDL_Scancode Key_Atack_id;
	SDL_Scancode Key_Ability_Save_id;
	SDL_Scancode Key_Ability_Load_id;

public: 
	Player(SDL_Scancode Key_Up_id = SDL_SCANCODE_UP, SDL_Scancode Key_Down_id = SDL_SCANCODE_DOWN, SDL_Scancode Key_Left_id = SDL_SCANCODE_LEFT, SDL_Scancode Key_Right_id = SDL_SCANCODE_RIGHT, SDL_Scancode Key_Atack_id = SDL_SCANCODE_SPACE, SDL_Scancode Key_Ability_Save_id = SDL_SCANCODE_Z, SDL_Scancode Key_Ability_Load_id = SDL_SCANCODE_X);
	~Player() = default;
	

	virtual void Update();
	virtual void Movement();
	virtual void Jump();
	//virtual void Atack();
	virtual void Ability();
	virtual void IsDiyng();
	void Delete()
	{}
};
