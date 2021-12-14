#include "Archer.h"

bool Archer::Parse(XMLElement* root, int iObject)
{
    if (!AnimatedObj::Parse(root, iObject)) { return false; }

    XMLElement* xmlElem = root->FirstChildElement("object");
    for (int i = 0; i < iObject; ++i)
    {
        xmlElem = xmlElem->NextSiblingElement();
    }

    if(!arrow.Parse(root, xmlElem->UnsignedAttribute("projectileType"))) { return false; }


    return true;
}

void Archer::Atack()
{
    
}

void Archer::Update()
{
    Player::Update();
    Atack();
}
