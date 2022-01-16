#include "MovableObject.h"
#include "SystemTimer.h"

bool MovableObject::Parse(XMLElement* root, int iObject, XMLElement* xmlElem)
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
			SDL_Log("Invalid Parameters for parsing that object");
			return false;
		}
	}


	if (!GObject::Parse(root, iObject, xmlElem)) { return false; }

	xmlElem->QueryFloatAttribute("vx", &MovableSpeed);

	return true;
}

void MovableObject::Movement()
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

	if (vx > 0 && !(direction == Direction::Left && collide.Left) && !(direction == Direction::Right && collide.Right))
	{
		vx *= 0.9;
		hitbox.x += (int)direction * vx * dt;
	}
	else
	{
		vx = 0.f;
	}
}

void MovableObject::Move(Direction direction)
{
	vx = MovableSpeed;
	this->direction = direction;
}

void MovableObject::Update()
{
	Movement();
}