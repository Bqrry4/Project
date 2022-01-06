#pragma once
#include "TextureManager.h"
#include "Vendor/tinyxml2.h"

using namespace tinyxml2;


struct Hitbox	//{x,y,w,h} coords
{
	float x, y;
	int w, h;
};

struct CollideFlags //Colliding flags
{
	bool Is : 1;

	bool Above : 1;
	bool Below : 1;
	bool Right : 1;
	bool Left : 1;

	bool WithOthers : 1;
};

class GObject
{
protected:
	Hitbox hitbox;
	Uint16 type; //Texture id
	SDL_RendererFlip flip;
	
	Uint16 TOffsetX;
	Uint16 TOffsetY;

	
	CollideFlags collide;
	bool Interact;
	bool Existence;

	const static float Gravity;

public:
	GObject() : hitbox({ 0,0,0,0 }), type(0), flip(SDL_FLIP_NONE), TOffsetX(0), TOffsetY(0), Existence(true), collide({ false }), Interact(false)
	{}
	virtual ~GObject() = default;

	virtual void Update() = 0;
	virtual void Draw(const SDL_Point* CameraTranslate = nullptr);
	virtual bool Parse(XMLElement* root, int iObject = 0, XMLElement* xmlElem = nullptr);

	inline Hitbox* GetHitbox() { return & hitbox; }
	inline bool IsColliding() { return collide.Is; }
	inline bool IsCollidingWithObj() { return collide.WithOthers; }
	inline bool IsInteracting() { return Interact; }

	void TranslateTo(SDL_Point);

	void setFlagAbove(bool value) { collide.Above = value; }
	void setFlagBelow(bool value) { collide.Below = value; }
	void setFlagRight(bool value) { collide.Right = value; }
	void setFlagLeft(bool value) { collide.Left = value; }
	void SetFlipMode(SDL_RendererFlip value) { flip = value; }

	bool shouldExist() { return Existence; }
	void SetExistenceMode(bool value) { Existence = value; }
};
