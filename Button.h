#pragma once
#include "TextureManager.h"

class Button
{
	SDL_Texture* texture;
	SDL_Rect Position;

	bool IsActive;

	void(*fun)(void);

public:
	Button() : texture(nullptr), Position({0,0,0,0}), IsActive(false), fun(nullptr)
	{}
	Button(SDL_Rect Position, const char* label, void(*fun)(void), bool active);
	~Button()
	{
		SDL_DestroyTexture(texture);
	}

	inline void SwitchActiveMode() { IsActive = !IsActive; }

	void CheckForPress();

	void Draw();
	void Update();
};

