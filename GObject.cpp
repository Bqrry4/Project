#include "GObject.h"

GObject::GObject(Uint16 type, Hitbox hitbox)
{
	this->hitbox = hitbox;
	this->type = type;
	//this->ObjState = ObjState;
}

void GObject::Draw() //Need testing
{
	TextureManager::GetInstance()->Draw(type, hitbox.x, hitbox.y, hitbox.w + 2*TOffsetX, hitbox.h+2*TOffsetY, flip);
}

void GObject::Update()
{



}

bool GObject::Parse(XMLElement* root, int iObject)
{

	XMLElement* xmlElem = root->FirstChildElement("object");
	for (int i = 0; i < iObject; ++i)
	{
		xmlElem = xmlElem->NextSiblingElement();
	}

	xmlElem->QueryFloatAttribute("x", &hitbox.x);
	xmlElem->QueryFloatAttribute("y", &hitbox.y);
	xmlElem->QueryIntAttribute("w", &hitbox.w);
	xmlElem->QueryIntAttribute("h", &hitbox.h);
	//xmlElem->QueryIntAttribute("TextureOffsetX", (int*)&TOffsetX);
	//xmlElem->QueryIntAttribute("TextureOffsetY", (int*)&TOffsetY);


	type = xmlElem->UnsignedAttribute("TexureId");

	TextureManager::GetInstance()->Load(xmlElem->Attribute("Texture"), type);

	flip = (SDL_RendererFlip)xmlElem->UnsignedAttribute("rotation");

	return true;
}
