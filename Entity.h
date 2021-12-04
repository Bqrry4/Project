#pragma once

class Entity
{
protected:
	int HP, AP;

public:
	Entity() : HP(0), AP(0)
	{}
	virtual void Movement() = 0;
	virtual void Atack() = 0;

};
