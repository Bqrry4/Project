#include "GObject.h"

//GObject::GObject(Uint16 type, Hitbox hitbox)
//{
//	this->hitbox = hitbox;
//	this->type = type;
//	//this->ObjState = ObjState;
//}

void GObject::Draw(SDL_Point* CameraTranslate) //Need testing
{
	TextureManager::GetInstance()->Draw(type, hitbox.x, hitbox.y, hitbox.w + 2*TOffsetX, hitbox.h+2*TOffsetY, 0, 0, flip, CameraTranslate);
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

	TOffsetX = xmlElem->UnsignedAttribute("TextureOffsetX");
	TOffsetY = xmlElem->UnsignedAttribute("TextureOffsetY");
	Spacing = xmlElem->UnsignedAttribute("Spacing");
	type = xmlElem->UnsignedAttribute("TextureId");

	flip = (SDL_RendererFlip)xmlElem->UnsignedAttribute("rotation");

	return true;
}
