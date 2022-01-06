#include "GObject.h"

//GObject::GObject(Uint16 type, Hitbox hitbox)
//{
//	this->hitbox = hitbox;
//	this->type = type;
//	//this->ObjState = ObjState;
//}

bool GObject::Parse(XMLElement* root, int iObject, XMLElement* xmlElem)
{
	if (xmlElem == nullptr)
	{
		xmlElem = root->FirstChildElement("object");
		for (int i = 0; i < iObject; ++i)
		{
			xmlElem = xmlElem->NextSiblingElement();
		}
	}

	xmlElem->QueryFloatAttribute("x", &hitbox.x);
	xmlElem->QueryFloatAttribute("y", &hitbox.y);
	xmlElem->QueryIntAttribute("w", &hitbox.w);
	xmlElem->QueryIntAttribute("h", &hitbox.h);

	TOffsetX = xmlElem->UnsignedAttribute("TextureOffsetX");
	TOffsetY = xmlElem->UnsignedAttribute("TextureOffsetY");

	type = xmlElem->UnsignedAttribute("TextureId");

	flip = (SDL_RendererFlip)xmlElem->UnsignedAttribute("rotation");

	return true;
}

void GObject::Draw(const SDL_Point* CameraTranslate) //Need testing
{
	TextureManager::GetInstance().Draw(type, { 0,0, hitbox.w + 2 * TOffsetX , hitbox.h + 2 * TOffsetY }, { (int)hitbox.x, (int)hitbox.y,hitbox.w + 2 * TOffsetX , hitbox.h + 2 * TOffsetY }, flip, CameraTranslate);
}

void GObject::TranslateTo(SDL_Point point)
{
	hitbox.x = point.x;
	hitbox.y = point.y;
}

void GObject::Update()
{



}