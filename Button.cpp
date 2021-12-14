#include "Button.h"
//#include "Menu.h"
#include "Input.h"


Button::Button(SDL_Rect Position, const char* label, void(*fun)(void), bool active)
{
	this->Position.x = Position.x;
	this->Position.y = Position.y;
	this->Position.w = Position.w;
	this->Position.h = Position.h;

	this->fun = fun;

	texture = TextureManager::GetInstance().Load(label, { 0xFF, 0xFF, 0xFF, 0xFF}, Position); //Check for nullptr
	//texture = TextureManager::GetInstance().FillTransparent(texture, { 0xFF, 0xFF, 0xFF, 0xFF }, Position);
	IsActive = active;

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}
//
//void Button::Draw()
//{}

void Button::CheckForPress()
{

	SDL_Point* mouse = Input::GetInstance()->MousePosition();

	if (mouse->x > Position.x && mouse->x < Position.x + Position.w && mouse->y > Position.y && mouse->y < Position.y + Position.h)
	{
		SDL_SetTextureColorMod(texture, 0xBB, 0xDD, 0xCC);
		SDL_SetTextureAlphaMod(texture, 0xBB);

		if (Input::GetInstance()->MouseClicked())
		{
			(*fun)();
		}
	}
	else
	{
		SDL_SetTextureColorMod(texture, 0xFF, 0xFF, 0xFF);
		SDL_SetTextureAlphaMod(texture, 0xFF);
	}
}

void Button::Draw()
{
	if (IsActive)
	{
		TextureManager::GetInstance().Draw(texture, Position.x, Position.y, Position.w, Position.h);
	}
}

void Button::Update()
{
	if (IsActive)
	{
		CheckForPress();
	}
}
