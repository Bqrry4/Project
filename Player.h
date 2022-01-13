#pragma once
#include "Entity.h"
#include "AnimatedObj.h"

enum class PlayerState {
	Repaos,
	Run,
	Jump,
	Atack,
	Ability,
	Dying
};


class Player : public AnimatedObj , public Entity
{
	float vx;
	float vy;

	float a; //Coeficientul de alunecare

	SDL_Point CheckPointPosition;
	float AbilityDT;

	int HangRange;
	bool HangMode;

protected:
	SDL_Scancode Key_Up_id;
	SDL_Scancode Key_Down_id;
	SDL_Scancode Key_Left_id;
	SDL_Scancode Key_Right_id;
	SDL_Scancode Key_Atack_id;
	SDL_Scancode Key_Ability_Save_id;
	SDL_Scancode Key_Ability_Load_id;
	SDL_Scancode Key_Hanging_id;

	virtual void Movement();
	virtual void Jump();
	//virtual void Atack();
	virtual void Ability();
public: 
	Player(SDL_Scancode Key_Up_id = SDL_SCANCODE_UP, SDL_Scancode Key_Down_id = SDL_SCANCODE_DOWN, SDL_Scancode Key_Left_id = SDL_SCANCODE_LEFT, SDL_Scancode Key_Right_id = SDL_SCANCODE_RIGHT, SDL_Scancode Key_Atack_id = SDL_SCANCODE_SPACE, SDL_Scancode Key_Ability_Save_id = SDL_SCANCODE_Z, SDL_Scancode Key_Ability_Load_id = SDL_SCANCODE_X, SDL_Scancode Key_Hanging_id = SDL_SCANCODE_C);
	~Player() = default;

	static bool PlayerDead;
	
	virtual void Update();
	virtual void IsDiyng();

	virtual bool IsDead() { return ObjState == (Uint16)PlayerState::Dying; }
	inline int getHangRange() { return HangRange; }
	inline bool IsHanging() { return HangMode; }
};
