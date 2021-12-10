#include "ProjectileObject.h"

bool ProjectileObject::Parse(XMLElement* root, int iObject)
{
    if (root == nullptr) { return false; }

    XMLElement* xmlElem = root->FirstChildElement("Projectile");
    for (int i = 0; i < iObject; ++i)
    {
        xmlElem = xmlElem->NextSiblingElement();
    }
    xmlElem->QueryIntAttribute("w", &hitbox.w);
    xmlElem->QueryIntAttribute("h", &hitbox.h);
    xmlElem->QueryFloatAttribute("vx", &vx);

    TOffsetX = xmlElem->UnsignedAttribute("TextureOffsetX");
    TOffsetY = xmlElem->UnsignedAttribute("TextureOffsetY");
    Spacing = xmlElem->UnsignedAttribute("Spacing");
    type = xmlElem->UnsignedAttribute("TextureId");

    return true;
}