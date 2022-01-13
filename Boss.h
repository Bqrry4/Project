#pragma once
#include "NPC.h"
#include "ProjectileObject.h"

enum class Positioning{
	Top,
	Middle,
	Bottom
};

class Boss : public NPC
{
	int MaxHealth;
	int SecondATRange = 300;

	void DrawHealthBar();
protected:
	bool Summoned;

public:
	Boss() : Summoned(false), MaxHealth(0)
	{
		Interact = false;
		~collide;
	}
	virtual ~Boss() = default;

	static bool IsDefeated;

	virtual void Draw(const SDL_Point* CameraTranslate = nullptr);
	virtual void Update();

	inline int getSecondRange() { return SecondATRange; }

	virtual void Atack() = 0; //Its an abstract class
	virtual bool IsDead() = 0;
};

class DeathBringer : public Boss
{
	enum States {
		Repaos,
		Run,
		Atack1,
		Atack2,
		Ability,
		Dying
	};

	AnimatedObj* Ability_Hand;

	bool AbilityAtackIntention;
	bool AtackingWithAbility;
	bool AbilitySpawnCoordsSet;

	void Atack();
	void IsDiyng();
public:
	DeathBringer() : Ability_Hand(nullptr), AbilityAtackIntention(false), AtackingWithAbility(false), AbilitySpawnCoordsSet(false)
	{}
	~DeathBringer()
	{
		delete Ability_Hand;
	}
	
	bool Parse(XMLElement* root, int iObject = 0, XMLElement* xmlElem = nullptr);
	void Draw(const SDL_Point* CameraTranslate = nullptr);
	void Update();
	bool IsDead() { return ObjState == Dying; }

	void WantToUseAbility(bool value) { AbilityAtackIntention = value;}
	bool IsAbilitySpawnCoordsSet() { return AbilitySpawnCoordsSet; }
	void SetAbilitySpawnCoords(SDL_Point point) { Ability_Hand->TranslateTo(point); AbilitySpawnCoordsSet = true; }
	inline bool UsingAbility() { return	AtackingWithAbility; };
	Hitbox* getAbilityHitbox() { return Ability_Hand->GetHitbox(); }
};


class SlimeBoss : public Boss
{
	enum States {
		Repaos,
		Run,
		Atacking,
		TakeDamage,
		Dying
	};

	ProjectileObject fireball;

	void Atack();
	void IsDiyng();
public:
	SlimeBoss()
	{}
	~SlimeBoss() = default;

	bool Parse(XMLElement* root, int iObject = 0, XMLElement* xmlElem = nullptr);
	void Update();

	ProjectileObject getProjectile(Positioning);
	bool IsDead() { return ObjState == Dying; }
};

class FireDemon : public Boss
{
	enum States {
		Repaos,
		Run,
		Atacking,
		TakeDamage,
		Dying
	};

	ProjectileObject wolf;
public:
	FireDemon()
	{}
	~FireDemon() = default;

	ProjectileObject getProjectile();

	void Atack()
	{}
	bool IsDead() { return ObjState == Dying; }
};