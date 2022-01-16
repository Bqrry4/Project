#include "NPC.h"
#include "SystemTimer.h"
#include "Level.h"
#include <string>

const float SpeedX = 50;


bool NPC::Parse(XMLElement* root, int iObject, XMLElement* xmlElem)
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
		AnimatedObj::Parse(root, iObject, xmlElem);
	}
	catch (std::string s) { throw s; }
	
	xmlElem->QueryIntAttribute("HP", &HP);
	xmlElem->QueryIntAttribute("AP", &AP);

	xmlElem->QueryIntAttribute("AtackRange", &AtRange);

	if (HP <= 0 || AP <= 0 || AtRange < 0)
		throw std::string("Invalid Object parameters \n");

	HP *= Level::DifficultyAmpl;
	AP *= Level::DifficultyAmpl;


	const char* type = xmlElem->Attribute("drop");
	if (type != nullptr && !strcmp(type, "Yes"))
	{
		drop = new ElderScroll;

		try {
			drop->Parse(root, 0);
		}
		catch (std::string s) { throw s; }

	}

	return true;
}

void NPC::Movement()
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
		ObjState = (Uint16)NPCState::Repaos;
		deplDt += SystemTimer::GetInstance().GetDt() * 1000;
	}
	else
	{
		vx = (int)direction * SpeedX;
		ObjState = (Uint16)NPCState::Run;
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
	if(vx < 0.0f)
	{
		flip = SDL_FLIP_HORIZONTAL;
		//direction = Direction::Left;
	}

	if (collide.Left && collide.Right)
	{
		vx = 0.0f;
		ObjState = (Uint16)NPCState::Repaos;
	}

	hitbox.x += vx * dt;
}

void NPC::IsDiyng()
{
	if (HP <= 0)
	{
		if (ObjState != (Uint16)NPCState::Dying)
		{
			ObjState = (Uint16)NPCState::Dying;
			collide.WithOthers = false;
			AMode = true;
			frame.aFrame = 0;
		}
		else
		{
			//Delete the obj or not?
		}
	}

}

ElderScroll* NPC::GetDrop()
{
	if (drop) 
		drop->TranslateTo({ (int)hitbox.x , (int)hitbox.y });

	return drop;
}


void NPC::Update()
{
	IsDiyng();
	if (ObjState != (Uint16)NPCState::Atack && ObjState != (Uint16)NPCState::Dying) {
		Movement();
	}
}