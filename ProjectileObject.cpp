#include "ProjectileObject.h"
#include "SystemTimer.h"

bool ProjectileObject::Parse(XMLElement* root, int iObject, XMLElement* xmlElem)
{

    if (xmlElem == nullptr)
    {
        xmlElem = root->FirstChildElement("Projectile");
        for (int i = 0; i < iObject; ++i)
        {
            xmlElem = xmlElem->NextSiblingElement();
        }
    }

    if (!AnimatedObj::Parse(root, iObject, xmlElem)) { return false; }

    xmlElem->QueryIntAttribute("w", &hitbox.w);
    xmlElem->QueryIntAttribute("h", &hitbox.h);
    xmlElem->QueryFloatAttribute("vx", &vx);

    TOffsetX = xmlElem->UnsignedAttribute("TextureOffsetX");
    TOffsetY = xmlElem->UnsignedAttribute("TextureOffsetY");

    type = xmlElem->UnsignedAttribute("TextureId");

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
    float dt = SystemTimer::GetInstance()->GetDt();
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