#include "Boss.h"
#include "ElderScroll.h"
#include "Game.h"
#include "SystemTimer.h"

bool Boss::IsDefeated = false;

void Boss::Draw(const SDL_Point* CameraTranslate)
{
	if (Summoned)
	{
		NPC::Draw(CameraTranslate);
		if ( !IsDead() && hitbox.x - CameraTranslate->x > 0 && hitbox.x - CameraTranslate->x < Game::ScreenWidth() && hitbox.y - CameraTranslate->y > 0 && hitbox.y - CameraTranslate->y < Game::ScreenHeigth())
		{
			DrawHealthBar();
		}
	}
}
void Boss::DrawHealthBar()
{
	if (!MaxHealth) MaxHealth = HP;

	float ratio = (float)HP / MaxHealth;

	SDL_Texture* texture = TextureManager::FillTransparent(nullptr, { 0xFF, 0, 0, 0 }, { 0, 0, (int)((Game::ScreenWidth() - 80) * ratio), 40 });
	TextureManager::Draw(texture, { 0, 0, (int)((Game::ScreenWidth() - 80) * ratio), 40 }, { 40, 40, (int)((Game::ScreenWidth() - 80) * ratio), 20 });
	SDL_DestroyTexture(texture);
}
void Boss::Update()
{
	if (!ElderScroll::InstanceCount)
	{
		Interact = true;
		collide.Is = true;
		collide.WithOthers = true;
		Summoned = true;
	}
	else return;
}


void DeathBringer::Atack()
{
	if (AtackDT < 1200)
	{
		AtackDT += SystemTimer::GetInstance().GetDt() * 1000;
		return;
	}
	if (AtackIntention)
	{
		if (ObjState != Atack1)
		{
			ObjState = Atack1;
			AMode = true;
			frame.aFrame = 0;
		}
		else
		{
			if (frame.aFrame == frame.States[ObjState] - 1)
			{
				AMode = false;
				ObjState = (Uint16)NPCState::Repaos;
				AtackingMode = false;
				AtackDT = 0;
				AtackFrameTrigger = false;
				AtackIntention = false;

			}
			if (frame.aFrame == frame.States[ObjState] / 2 && !AtackFrameTrigger)
			{
				AtackingMode = true;
				AtackFrameTrigger = true;
			}
			else
			{
				AtackingMode = false;
			}
		}
	}
	else {
		if (AbilityAtackIntention)
		{
			if (ObjState != Atack2)
			{
				ObjState = Atack2;
				AMode = true;
				frame.aFrame = 0;
			}
			else
			{
				if (frame.aFrame == frame.States[ObjState] - 1)
				{
					AMode = false;
					ObjState = (Uint16)NPCState::Repaos;
					AtackingWithAbility = false;
					AtackDT = 0;
					AtackFrameTrigger = false;
					AbilityAtackIntention = false;
					AbilitySpawnCoordsSet = false;

				}
				if (frame.aFrame == frame.States[ObjState] / 2 && !AtackFrameTrigger)
				{
					AtackingWithAbility = true;
					AtackFrameTrigger = true;
				}
				else
				{
					AtackingWithAbility = false;
				}
			}
		}
	}
}
void DeathBringer::Draw(const SDL_Point* CameraTranslate)
{
	if (Summoned)
	{
		Boss::Draw(CameraTranslate);
		if (ObjState == Atack2)
		{
			Ability_Hand->Draw(CameraTranslate);
		}
	}
}
bool DeathBringer::Parse(XMLElement* root, int iObject, XMLElement* xmlElem)
{
	if (xmlElem == nullptr)
	{
		xmlElem = root->FirstChildElement("object");
		for (int i = 0; i < iObject; ++i)
		{
			xmlElem = xmlElem->NextSiblingElement();
		}

		if (xmlElem == nullptr)
		{
			throw std::string("Invalid Parameters for parsing that object");
		}
	}

	try {
		Boss::Parse(root, iObject, xmlElem);
		Ability_Hand = new AnimatedObj(*this);
		Ability_Hand->Parse(root, iObject, xmlElem);
		Ability_Hand->SetObjectState(Ability);
	}
	catch (std::string s) { throw s; }
	return true;
}
void DeathBringer::Update()
{
	if (!Summoned)
	{
		Boss::Update();
	}
	else
	{
		IsDiyng();
		if (ObjState != Dying) {

			if (ObjState != Atack1 && ObjState != Atack2) {
				Movement();
			}
			Atack();
		}
	}

}
void DeathBringer::IsDiyng()
{
	if (HP <= 0)
	{
		if (ObjState != Dying)
		{
			ObjState = Dying;
			collide.WithOthers = false;
			AMode = true;
			frame.aFrame = 0;
		}
		else
		{
			//Delete the obj or not?
			if (frame.aFrame == frame.States[ObjState] - 1)
			{
				Boss::IsDefeated = true;
			}
		}
	}
}

ProjectileObject SlimeBoss::getProjectile(Positioning id)
{
	SDL_Point point = { (int)hitbox.x, (int)hitbox.y };
	switch (id)
	{
	case Positioning::Top:
		(direction == Direction::Left) ? (point = { (int)hitbox.x, (int)hitbox.y }) : (point = { (int)hitbox.x + hitbox.w, (int)hitbox.y });
		break;
	case Positioning::Middle:
		(direction == Direction::Left) ? (point = { (int)hitbox.x, (int)hitbox.y + hitbox.h/2 }) : (point = { (int)hitbox.x + hitbox.w, (int)hitbox.y + hitbox.h / 2 });
		break;
	case Positioning::Bottom:
		(direction == Direction::Left) ? (point = { (int)hitbox.x, (int)hitbox.y + hitbox.h }) : (point = { (int)hitbox.x + hitbox.w, (int)hitbox.y + hitbox.h });
		break;
	}

	fireball.AdaptPosition(point, direction);

	return fireball;
}
bool SlimeBoss::Parse(XMLElement* root, int iObject, XMLElement* xmlElem)
{
	if (xmlElem == nullptr)
	{
		xmlElem = root->FirstChildElement("object");
		for (int i = 0; i < iObject; ++i)
		{
			xmlElem = xmlElem->NextSiblingElement();
		}

		if (xmlElem == nullptr)
		{
			throw std::string("Invalid Parameters for parsing that object");
		}
	}

	try {
		Boss::Parse(root, iObject, xmlElem);
		fireball.Parse(root, xmlElem->UnsignedAttribute("projectileType"));
	}
	catch (std::string s) { throw s; }

	return true;
}
void SlimeBoss::Update()
{
	if (!Summoned)
	{
		Boss::Update();
	}
	else
	{
		IsDiyng();
		if (ObjState != Dying) {

			if (ObjState != Atacking) {
				Movement();
			}
			Atack();
		}
	}
}
void SlimeBoss::IsDiyng()
{
	if (HP <= 0)
	{
		if (ObjState != Dying)
		{
			ObjState = Dying;
			collide.WithOthers = false;
			AMode = true;
			frame.aFrame = 0;
		}
		else
		{
			//Delete the obj or not?
			if (frame.aFrame == frame.States[ObjState] - 1)
			{
				Boss::IsDefeated = true;
			}
		}
	}
}
void SlimeBoss::Atack()
{
	if (AtackDT < 900)
	{
		AtackDT += SystemTimer::GetInstance().GetDt() * 1000;
		return;
	}
	if (AtackIntention)
	{
		if (ObjState != Atacking)
		{
			ObjState = Atacking;
			AMode = true;
			frame.aFrame = 0;
		}
		else
		{
			if (frame.aFrame == frame.States[ObjState] - 1)
			{
				AMode = false;
				ObjState = (Uint16)NPCState::Repaos;
				AtackingMode = false;
				AtackDT = 0;
				AtackFrameTrigger = false;
				AtackIntention = false;

			}
			if (frame.aFrame == frame.States[ObjState] / 2 && !AtackFrameTrigger)
			{
				AtackingMode = true;
				AtackFrameTrigger = true;
			}
			else
			{
				AtackingMode = false;
			}
		}
	}
}

ProjectileObject FireDemon::getProjectile()
{
	SDL_Point point;
	(direction == Direction::Left) ?
		(point = { (int)hitbox.x + hitbox.w, (int)hitbox.y + hitbox.h / 2 })
		: (point = { (int)hitbox.x, (int)hitbox.y + hitbox.h / 2 });
	wolf.AdaptPosition(point, direction);

	return wolf;
}
void FireDemon::Atack()
{
	if (AtackDT < 1100)
	{
		AtackDT += SystemTimer::GetInstance().GetDt() * 1000;
		return;
	}
	if (AtackIntention)
	{
		if (ObjState != Atacking)
		{
			ObjState = Atacking;
			AMode = true;
			frame.aFrame = 0;
		}
		else
		{
			if (frame.aFrame == frame.States[ObjState] - 1)
			{
				AMode = false;
				ObjState = Levitate;
				AtackingMode = false;
				AtackDT = 0;
				AtackFrameTrigger = false;
				AtackIntention = false;

			}
			if (frame.aFrame == frame.States[ObjState] / 2 && !AtackFrameTrigger)
			{
				AtackingMode = true;
				AtackFrameTrigger = true;
			}
			else
			{
				AtackingMode = false;
			}
		}
	}
}
void FireDemon::Movement()
{
	float dt = SystemTimer::GetInstance().GetDt();

	if (!collide.Below)
	{
		vy += Gravity * dt;
		hitbox.y += vy * dt;
	}
	else
	{
		vy = 0.f;
	}

	if (deplDt < 500)
	{
		vx = 0.0f;
		ObjState = Levitate;
		deplDt += SystemTimer::GetInstance().GetDt() * 1000;
	}
	else
	{
		vx = (int)direction * 50;
		ObjState = Levitate;
		if (abs(LastXPosition - hitbox.x) > deplasament || (collide.Right && direction == Direction::Right) || (collide.Left && direction == Direction::Left))
		{
			deplDt = 0;
			vx *= -1;
			direction = ~direction;
			RandDeplasament();
			LastXPosition = hitbox.x;
		}
	}


	if (vx > 0.0f)
	{
		flip = SDL_FLIP_NONE;
		//direction = Direction::Right;
	}
	if (vx < 0.0f)
	{
		flip = SDL_FLIP_HORIZONTAL;
		//direction = Direction::Left;
	}

	if (collide.Left && collide.Right)
	{
		vx = 0.0f;
		ObjState = Levitate;
	}

	hitbox.x += vx * dt;
}
bool FireDemon::Parse(XMLElement* root, int iObject, XMLElement* xmlElem)
{
	if (xmlElem == nullptr)
	{
		xmlElem = root->FirstChildElement("object");
		for (int i = 0; i < iObject; ++i)
		{
			xmlElem = xmlElem->NextSiblingElement();
		}

		if (xmlElem == nullptr)
		{
			throw std::string("Invalid Parameters for parsing that object");
		}
	}

	try {
		Boss::Parse(root, iObject, xmlElem);
		wolf.Parse(root, xmlElem->UnsignedAttribute("projectileType"));
	}
	catch (std::string s) { throw s; }

	return true;
}
void FireDemon::Update()
{
	if (!Summoned)
	{
		Boss::Update();
	}
	else
	{
		IsDiyng();
		if (ObjState != Dying) {

			if (ObjState != Atacking) {
				Movement();
			}
			Atack();
		}
	}
}
