#pragma once
#include "Player.h"
#include "ProjectileObject.h"

enum class ArcherSideState {
	AtackLow = 5,
	AtackHigh
};

class Archer : public Player
{
	ProjectileObject arrow;
	SDL_Point pos1;

	//void AdaptProjectile(); //Adapt projectile position to Archer's
public:
	Archer() : Player(), pos1({0,0})
	{}
	bool Parse(XMLElement* root, int iObject = 0, XMLElement* xmlElem = nullptr);
	void Atack();
	void Update();

	inline ProjectileObject getProjectile() {  return arrow; }
};

