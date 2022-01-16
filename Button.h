#pragma once
#include "TextureManager.h"
#include "SoundManager.h"

class Button
{
	int id;
	Uint32 EventID;

	SDL_Texture* texture;
	SDL_Rect Position;

	bool IsActive;

public:
	Button() : id(0), texture(nullptr), Position({0,0,0,0}), IsActive(false), EventID(0)
	{}
	Button(int id, SDL_Rect Position, const char* label, bool active, Uint32 EventID);
	~Button()
	{
		SDL_DestroyTexture(texture);
	}

	inline void SwitchActiveMode() { IsActive = !IsActive; }
	inline void SetActiveMode(bool value) { IsActive = value; }

	void CheckForPress(SDL_Point* mouse, bool press);
	void Draw();

	//void Update();
};

