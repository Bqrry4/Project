#include "Button.h"
//#include "Menu.h"
#include "Input.h"

Button::Button(int id, SDL_Rect Position, const char* label, bool active, Uint32 EventID)
{
	this->id = id;
	this->EventID = EventID;

	this->Position.x = Position.x;
	this->Position.y = Position.y;
	this->Position.w = Position.w;
	this->Position.h = Position.h;


	texture = TextureManager::GetInstance().Load(label, { 0xFF, 0xFF, 0xFF, 0xFF}, Position); //Check for nullptr
	texture = TextureManager::FillTransparent(texture, { 0, 0, 0, 0xFF }, Position);
	IsActive = active;

	SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
}


void Button::CheckForPress(SDL_Point* mouse, bool MouseClick)
{
	if (IsActive)
	{
		if (mouse->x > Position.x && mouse->x < Position.x + Position.w && mouse->y > Position.y && mouse->y < Position.y + Position.h)
		{
			SDL_SetTextureColorMod(texture, 0xBB, 0xDD, 0xCC);
			SDL_SetTextureAlphaMod(texture, 0xBB);

			if (MouseClick)
			{
				SDL_Event event;
				SDL_zero(event);
				event.type = EventID;
				event.user.code = id;
				event.user.data1 = 0;
				event.user.data2 = 0;
				SDL_PushEvent(&event);
			}
		}
		else
		{
			SDL_SetTextureColorMod(texture, 0xFF, 0xFF, 0xFF);
			SDL_SetTextureAlphaMod(texture, 0xFF);
		}
	}
}

void Button::Draw()
{
	if (IsActive)
	{
		TextureManager::Draw(texture, { 0, 0, Position.w, Position.h }, { Position.x, Position.y, Position.w, Position.h });
	}
}

//void Button::Update()
//{
//	if (IsActive)
//	{
//		CheckForPress();
//	}
//}
