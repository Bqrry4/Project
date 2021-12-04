#pragma once
#include "TextureManager.h"
#include "Vendor/tinyxml2.h"

using namespace tinyxml2;

//Interface of Object
class IObject {

public:
	virtual void Update() = 0;
	virtual void Draw() = 0;
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

class GObject : public IObject
{
protected:

		//Properties of an Static Object
		Hitbox hitbox;
		Uint16 type; //Texture id
		SDL_RendererFlip flip; //Flipping texture
		
		__int8 TOffsetX;
		__int8 TOffsetY;

		bool collide;

		bool colideAbove;
		bool colideBelow;
		bool colideRight;
		bool colideLeft;
public:
	GObject() : hitbox({ 0,0,0,0 }), type(0), flip(SDL_FLIP_NONE), collide(true), colideAbove(false), colideBelow(false), colideRight(false), colideLeft(false), TOffsetX(0), TOffsetY(0)
	{}
	GObject(Uint16 type, Hitbox box);

	virtual void Update();
	virtual void Draw();
	//virtual void Delete();
	virtual bool Parse(XMLElement* root, int iObject = 0);

	inline Hitbox* GetHitbox() { return & hitbox; }
	inline bool IsColliding() { return collide; }

	void setFlagAbove(bool value) { colideAbove = value; }
	void setFlagBelow(bool value) { colideBelow = value; }
	void setFlagRight(bool value) { colideRight = value; }
	void setFlagLeft(bool value) { colideLeft = value; }
};
