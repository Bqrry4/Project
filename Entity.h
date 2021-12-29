#pragma once
#include<SDL.h>
enum class Looking
{
	Left = -1,
	Right = 1
};

class Entity
{
protected:
	int HP, AP;
	Looking direction;
	int AtRange;
	bool AtackingMode : 1;
	int AtackDT;

	bool Respaunable : 1;
	bool AtackFrameTrigger : 1;
	int AtackFrame;
public:
	Entity() : HP(0), AP(0), direction(Looking::Left), AtRange(0), AtackingMode(false), AtackDT(0), AtackFrameTrigger(false)
	{}
	inline Looking ViewDirection() { return direction; }
	inline int DoDamage() { return AP; }
	inline void TakeDamage(int Damage) { SDL_Log("%d", HP); HP -= Damage; }
	inline int AtackRange() { return AtRange; }

	inline bool IsAtacking() { return AtackingMode; }
	//inline void IsAtacking(bool value) {AtackingMode = value; }

	virtual void Movement() = 0;
	virtual void Atack() = 0;
	virtual void IsDiyng() = 0;

};
