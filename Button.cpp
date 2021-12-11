#include "Button.h"
#include <SDL_ttf.h>

Button::Button(SDL_Rect Position, void(*fun)(void), const char* label)
{
	this->Position.x = Position.x;
	this->Position.y = Position.y;
	this->Position.w = Position.w;
	this->Position.h = Position.h;
	this->fun = fun;


}

void Button::Draw()
{}
void Button::CheckForPress()
{}