#include <iostream>
#include <string>
#include <sstream>
#include "AnimatedObj.h"

void AnimatedObj::Draw()
{

	if (ObjState > frame.aStates) ObjState = 0;

	frame.aFrame = (SDL_GetTicks() / frame.AnimSpeed) % frame.States[ObjState];
	TextureManager::GetInstance()->Draw(type, hitbox.x, hitbox.y, hitbox.w, hitbox.h, ObjState, frame.aFrame, flip);

}

void AnimatedObj::Update()
{



}

bool AnimatedObj::Parse(XMLElement* root, int iObject)
{

	if (!GObject::Parse(root, iObject)) { return false; }

	XMLElement* xmlElem = root->FirstChildElement("object");
	for (int i = 0; i < iObject; ++i)
	{
		xmlElem = xmlElem->NextSiblingElement();
	}


	frame.aStates = xmlElem->UnsignedAttribute("states");
	frame.States = new Uint8[frame.aStates];


	std::string vector = xmlElem->Attribute("frames");
	std::istringstream stream(vector);
	std::string value;

	for (int i = 0; i < frame.aStates; ++i)
	{
		std::getline(stream, value, ';');
		frame.States[i] = std::stoi(value);

		if (!stream.good()) break;

	}

	return true;
}