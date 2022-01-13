#pragma once
#include "GObject.h"


class AnimatedObj : public GObject {
private:
	int timeElapsed;
	void NextFrame();

protected:
	struct Frame {		//Frame texture info
		Uint8* States;
		Uint8 aStates;
		Uint8 aFrame;
		int AnimSpeed;
	} frame;

	Uint16 ObjState;

	bool AMode : 1; // When true, animation will get stuck on the last frame

public:
	AnimatedObj() : GObject(), frame({ nullptr, 0, 0, 75 }) , ObjState(0), AMode(false), timeElapsed(0)
	{}
	virtual ~AnimatedObj();
	AnimatedObj(const AnimatedObj& obj);

	virtual void Draw(const SDL_Point* CameraTranslate = nullptr);
	virtual void Update();
	virtual bool Parse(XMLElement* root, int iObject = 0, XMLElement* xmlElem = nullptr);

	inline void SetObjectState(Uint16 state) { ObjState = state; }
};

