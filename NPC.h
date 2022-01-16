#pragma once
#include "AnimatedObj.h"
#include "Entity.h"
#include "ElderScroll.h"


enum class NPCState {
	Repaos,
	Run,
	Atack,
	Dying,
	Dead
};

class NPC : public AnimatedObj, public Entity
{
	ElderScroll* drop;

protected:
	float vx, vy;

	float LastXPosition;
	int deplasament, deplDt;

	bool AtackIntention : 1;

	void RandDeplasament() { deplasament = abs((rand() % 5) * (int)hitbox.x /* + (int)hitbox.x*/); }
	virtual void Movement();
	virtual void IsDiyng();

public:
	NPC() : AnimatedObj(), vx(0.f), vy(0.f), LastXPosition(0), deplasament(0), deplDt(0), AtackIntention(false), drop(nullptr)
	{
		Interact = true;
		collide.Is = true;
		collide.WithOthers = true;
	}
	virtual ~NPC()
	{
		delete drop;
	}

	void WantToAtack(bool value) { AtackIntention = value;}
	virtual bool IsDead() { return ObjState == (Uint16)NPCState::Dying; }
	ElderScroll* GetDrop();

	void Update();
	bool Parse(XMLElement* root, int iObject = 0, XMLElement* xmlElem = nullptr);

};

