#include "ProjectileObject.h"
#include "SystemTimer.h"
#include <string>

bool ProjectileObject::Parse(XMLElement* root, int iObject, XMLElement* xmlElem)
{

    if (xmlElem == nullptr)
    {
        xmlElem = root->FirstChildElement("Projectile");
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

    xmlElem->QueryFloatAttribute("vx", &vx);

    return true;
}

void ProjectileObject::AdaptPosition(SDL_Point pos, Direction direction)
{
    TranslateTo(pos);
    this->direction = direction;
    flip = (direction == Direction::Left) ? (SDL_FLIP_HORIZONTAL) : (SDL_FLIP_NONE);
}


void ProjectileObject::Movement()
{
    float dt = SystemTimer::GetInstance().GetDt();
    hitbox.x += (int)direction * vx * dt;
}

void ProjectileObject::Update()
{
    Movement();

    if (direction == Direction::Left && collide.Left || direction == Direction::Right && collide.Right)
    {
        Existence = false;
    }
}