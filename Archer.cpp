#include "Archer.h"
#include "Input.h"
#include "SystemTimer.h"

bool Archer::Parse(XMLElement* root, int iObject, XMLElement* xmlElem)
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

    xmlElem->QueryIntAttribute("projectilespawnPositionX", &pos1.x);
    xmlElem->QueryIntAttribute("projectilespawnPositionY", &pos1.y);

    if(!arrow.Parse(root, xmlElem->UnsignedAttribute("projectileType"))) { return false; }


    return true;
}

void Archer::Atack()
{
    if (AtackDT < 500) //  Swordsman atack delay 0.5 sec
    {
        AtackDT += SystemTimer::GetInstance()->GetDt() * 1000;
        return;
    }
    if (ObjState != (Uint16)PlayerState::Atack) {

        if (Input::GetInstance().KeyState(Key_Atack_id))
        {
            ObjState = (Uint16)PlayerState::Atack;
            AMode = true;
            frame.aFrame = 0;
        }
    }
    else
    {
        if (frame.aFrame == frame.States[ObjState] - 1)
        {
            AMode = false;
            ObjState = (Uint16)PlayerState::Repaos;
            AtackDT = 0;
            AtackFrameTrigger = false;
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

void Archer::AdaptProjectile()
{
    if (direction == Direction::Left)
    {
        arrow.SetFlipMode(SDL_FLIP_HORIZONTAL);
        arrow.TranslateTo({ (int)hitbox.x + hitbox.w - pos1.x, (int)hitbox.y + pos1.y });
        arrow.SetDirection(Direction::Left);
    }
    if (direction == Direction::Right)
    {
        arrow.SetFlipMode(SDL_FLIP_NONE);
        arrow.TranslateTo({ (int)hitbox.x + pos1.x, (int)hitbox.y + pos1.y });
        arrow.SetDirection(Direction::Right);
    }
}

void Archer::Update()
{
    Player::Update();
    Atack();
    if (AtackingMode)
    {
        AdaptProjectile();
    }
}
