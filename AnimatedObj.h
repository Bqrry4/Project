#pragma once
#include "GObject.h"


class AnimatedObj : public GObject {
protected:

	struct Frame {		//Frame texture info
		Uint8* States;
		Uint8 aStates;
		Uint8 aFrame;
		int AnimSpeed;
	} frame;

	Uint16 ObjState;




public:
	AnimatedObj() : GObject(), frame({ nullptr, 0, 0, 50 }) , ObjState(0)
	{
		ObjectClassId = 1;
	}

	//AnimatedObj(Uint16 type, Hitbox hitbox, Uint8 row = 0, Uint8 column = 0, int AnimSpeed = 50, SDL_RendererFlip flip = SDL_FLIP_NONE) : GObject(type, hitbox)
	//{
	//	this->flip = flip;
	//	//this->frame.type = ftype;
	//	this->frame.aFrames = column;
	//	this->frame.AnimSpeed = AnimSpeed;
	//}

	virtual void Draw();
	virtual void Update();
	virtual bool Parse(XMLElement* root, int iObject = 0);
};

