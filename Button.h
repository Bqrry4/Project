#pragma once
#include <SDL.h>

class Button
{
	SDL_Texture* Texture;
	SDL_Rect Position;
	void(*fun)(void);

	bool IsActive;
public:
	Button(SDL_Rect, void(*)(void), const char* label);
	void Draw();
	void CheckForPress();
};

