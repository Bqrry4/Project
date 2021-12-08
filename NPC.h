#pragma once
#include "AnimatedObj.h"
#include "Entity.h"


enum class NPCState {
	Repaos,
	Run,
	Atack,
	Dying,
	Dead
};

class NPC : public AnimatedObj, public Entity
{

	float vx;

	float LastXPosition;
	int deplasament;
public:
	NPC() : vx(0), deplasament(0)
	{
		ObjectClassId = 3;

		collide.Is = true;
		collide.WithOthers = true;
		HP = rand() % 20 + 100;
		AP = 100;
		RandDeplasament();
		LastXPosition = hitbox.x;
	}

	void RandDeplasament() { deplasament = abs((rand() % 5 )* (int)hitbox.x + (int)hitbox.x); }

	virtual void Movement();
	virtual void Atack();
	virtual void IsDiyng();

	void Update();

};

