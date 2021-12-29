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
	float vx, vy;

	float LastXPosition;
	int deplasament, deplDt;

protected:
	bool AtackIntention : 1;

public:
	NPC() : AnimatedObj(), vx(0), vy(0), LastXPosition(0), deplasament(0), deplDt(0), AtackIntention(false)
	{
		ObjectClassId = 3;

		collide.Is = true;
		collide.WithOthers = true;
		HP = rand() % 20 + 100;

		AP = 50;
		AtRange = 12;
	}

	void RandDeplasament() { deplasament = abs((rand() % 5) * (int)hitbox.x /* + (int)hitbox.x*/); }
	void WantToAtack(bool value) { AtackIntention = value;}

	virtual void Movement();
	virtual void IsDiyng();

	void Update();

};

