#include <iostream>
#include <string>
#include <sstream>
#include "AnimatedObj.h"
#include "SystemTimer.h"

bool AnimatedObj::Parse(XMLElement* root, int iObject, XMLElement* xmlElem)
{

	if (xmlElem == nullptr)
	{
		xmlElem = root->FirstChildElement("object");
		for (int i = 0; i < iObject; ++i)
		{
			xmlElem = xmlElem->NextSiblingElement();
		}
	}

	if (!GObject::Parse(root, iObject, xmlElem)) { return false; }


	frame.aStates = xmlElem->UnsignedAttribute("states");
	frame.States = new Uint8[frame.aStates];


	std::string vector = xmlElem->Attribute("frames");
	std::istringstream stream(vector);
	std::string value;

	for (int i = 0; i < frame.aStates; ++i)
	{
		std::getline(stream, value, ';');
		int converter = std::stoi(value);
		frame.States[i] = (converter != 0) ? converter : 1;

	}

	return true;
}

void AnimatedObj::Draw(const SDL_Point* CameraTranslate)
{
	if (ObjState >= frame.aStates) { ObjState = 0; }
	NextFrame();

	TextureManager::GetInstance().Draw(type, { (hitbox.w + 2 * TOffsetX) * frame.aFrame, (hitbox.h + 2 * TOffsetY) * ObjState, hitbox.w + 2 * TOffsetX, hitbox.h + 2 * TOffsetY }, { (int)hitbox.x - TOffsetX, (int)hitbox.y - TOffsetY, hitbox.w + 2 * TOffsetX, hitbox.h + 2 * TOffsetY }, flip, CameraTranslate);
}

void AnimatedObj::Update()
{

}

void AnimatedObj::NextFrame()
{ 
	if (AMode)
	{
		if(frame.aFrame < frame.States[ObjState]-1)
		{
			timeElapsed += (int)(SystemTimer::GetInstance()->GetDt()*1000);
			if (timeElapsed / frame.AnimSpeed > 0)
			{
				frame.aFrame += timeElapsed / frame.AnimSpeed;
				timeElapsed = 0;
			}
		}
	} 
	else
	{
		frame.aFrame = (SDL_GetTicks() / frame.AnimSpeed) % frame.States[ObjState];
	}
}
