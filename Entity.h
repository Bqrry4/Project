#pragma once
#include<SDL.h>

enum class Direction : int
{
	Left = -1,
	Right = 1,
};
Direction operator~(Direction d); //Switch direction operator


class Entity
{
protected:
	int HP, AP;
	Direction direction;
	int AtRange;
	bool AtackingMode : 1;
	int AtackDT;

	bool Respaunable : 1;
	bool AtackFrameTrigger : 1;
	int AtackFrame;

	virtual void Movement() = 0;
	virtual void Atack() = 0;
	virtual void IsDiyng() = 0;
public:
	Entity() : HP(10), AP(10), direction(Direction::Left), AtRange(0), AtackingMode(false), AtackDT(0), AtackFrameTrigger(false)
	{}
	virtual ~Entity() = default;

	inline Direction ViewDirection() { return direction; }
	inline int DoDamage() { return AP; }
	inline void TakeDamage(int Damage) { SDL_Log("%d", HP); HP -= Damage; }
	inline int AtackRange() { return AtRange; }

	inline bool IsAtacking() { return AtackingMode; }
	inline void IsAtacking(bool value) {AtackingMode = value; }

	virtual bool IsDead() = 0;
};
