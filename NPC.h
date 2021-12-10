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

protected:
	bool AtackIntention : 1;

public:
	NPC() : AnimatedObj(), vx(0), deplasament(0), AtackIntention(false)
	{
		ObjectClassId = 3;

		collide.Is = true;
		collide.WithOthers = true;
		HP = rand() % 20 + 100;


		RandDeplasament();
		LastXPosition = hitbox.x;

		AP = 50;
		AtRange = 12;
	}

	void RandDeplasament() { deplasament = abs((rand() % 5 )* (int)hitbox.x + (int)hitbox.x); }
	void WantToAtack(bool value) { AtackIntention = value;}

	virtual void Movement();
	virtual void IsDiyng();

	void Update();

};

