#pragma once
#include "TextureManager.h"
#include "Vendor/tinyxml2.h"

using namespace tinyxml2;

//Interface of Object
class IObject {

public:
	virtual void Update() = 0;
	virtual void Draw(SDL_Point* CameraTranslate) = 0;
	//virtual void Delete() = 0;
	virtual bool Parse(XMLElement*, int) = 0;
	//virtual void Colision() = 0;

	virtual ~IObject() = default;

};



//GameObject 

struct Hitbox	//{x,y,w,h} coords
{
	float x;
	float y;
	int w;
	int h;
};

struct CollideFlags //Colliding flags
{
	bool Is : 1;

	bool Above : 1;
	bool Below : 1;
	bool Right : 1;
	bool Left : 1;

	bool WithOthers : 1;

	bool LeftObj : 1;
	bool RightObj : 1;
};

class GObject : public IObject
{
protected:
	__int8 ObjectClassId;

		//Properties of an Static Object
		Hitbox hitbox;
		Uint16 type; //Texture id
		SDL_RendererFlip flip; //Flipping texture
		
		Uint16 TOffsetX;
		Uint16 TOffsetY;
		Uint16 Spacing;

		CollideFlags collide;


public:
	GObject() : hitbox({ 0,0,0,0 }), type(0), flip(SDL_FLIP_NONE), TOffsetX(0), TOffsetY(0), Spacing(0)
	{
		ObjectClassId = 0;

		collide.Is = false;
		collide.Above = false;
		collide.Below = false;
		collide.Right = false;
		collide.Left = false;
		collide.WithOthers = false;
		collide.RightObj = false;
		collide.LeftObj = false;
	
	}
	//GObject(Uint16 type, Hitbox box);

	virtual void Update();
	virtual void Draw(SDL_Point* CameraTranslate = nullptr);
	//virtual void Delete();
	virtual bool Parse(XMLElement* root, int iObject = 0);

	inline __int8 GetObjectClassId() { return ObjectClassId; }

	inline Hitbox* GetHitbox() { return & hitbox; }
	inline bool IsColliding() { return collide.Is; }
	inline bool IsCollidingWithObj() { return collide.WithOthers; }


	void setFlagAbove(bool value) { collide.Above = value; }
	void setFlagBelow(bool value) { collide.Below = value; }
	void setFlagRight(bool value) { collide.Right = value; }
	void setFlagLeft(bool value) { collide.Left = value; }

	void setFlagRightWithObj(bool value) { collide.RightObj = value; }
	void setFlagLeftWithObj(bool value) { collide.LeftObj = value; }
};
