#pragma once
#include "Button.h"

class Menu
{
	SDL_Texture* Background;
	Button* buttons;

	bool MenuTrigger;
public:
	void Draw();
	void Update();
};

