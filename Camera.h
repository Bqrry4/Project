#pragma once
#include "GObject.h"

class Camera
{
	SDL_Point Position;

	int borderW;
	int	borderH;

public:
	Camera() : Position({ 0,0 }), borderW(0), borderH(0)
	{}
	Camera(int borderW, int borderH) : Position({ 0,0 }), borderW(borderW), borderH(borderH)
	{}
	~Camera() = default;

	void Update(Hitbox* target);

	inline SDL_Point* GetCameraPos() { return &Position; }
};

