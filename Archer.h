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
public:
	Archer() : Player()
	{}
	bool Parse(XMLElement* root, int iObject = 0, XMLElement* xmlElem = nullptr);
	void Atack();
	void Update();

	void AdaptProjectile(); //Adapt projectile position to Archer's
	inline ProjectileObject getProjectile() { return arrow; }
};

