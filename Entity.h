#pragma once
#include<SDL.h>
enum class Looking
{
	Left,
	Right
};

class Entity
{
protected:
	int HP, AP;
	Looking direction;
	int AtRange;
	bool AtackingMode;
	int AtackDT;
public:
	Entity() : HP(0), AP(0), direction(Looking::Left), AtRange(0), AtackingMode(0), AtackDT(0)
	{}
	inline Looking ViewDirection() { return direction; }
	inline int DoDamage() { return AP; }
	inline void TakeDamage(int Damage) { SDL_Log("%d", HP); HP -= Damage; }
	inline int AtackRange() { return AtRange; }
	inline bool IsAtacking() { return AtackingMode; }


	virtual void Movement() = 0;
	virtual void Atack() = 0;
	virtual void IsDiyng() = 0;

};
