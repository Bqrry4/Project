#include "Input.h"
#include "Game.h"

Input::Input()
{
	currentKeyStates = SDL_GetKeyboardState(NULL);
	//SDL_GetMouseState(&mouse.x, &mouse.y);
}

void Input::Read()
{
	SDL_Event ev;
	SDL_PollEvent(&ev);
	event = ev;

	switch (event.type) {
	case SDL_QUIT:
		Game::GetInstance().Quit();
		break;
	case SDL_KEYDOWN:
		UpdateKeyStates();
		break;
	case SDL_KEYUP:
		UpdateKeyStates();
		break;
	//case SDL_MOUSEBUTTONUP:
	//	MouseClick = true;
	//	break;
	//case SDL_MOUSEBUTTONDOWN:
	//	MouseClick = false;
	//	break;
	//case SDL_MOUSEMOTION:
	//	SDL_GetMouseState(&mouse.x, &mouse.y);
	//	break;
	}
}

bool Input::WasPressed(SDL_Scancode key)
{
	if (event.type == SDL_KEYUP && event.key.keysym.scancode == key)
	{
		return true;
	}
	return false;
}

void Input::UpdateKeyStates()
{
	currentKeyStates = SDL_GetKeyboardState(NULL);
}


bool Input::KeyState(SDL_Scancode key)
{
	return currentKeyStates[key];
}

//SDL_Point* Input::MousePosition()
//{
//	return &mouse;
//}
//bool Input::MouseClicked()
//{ 
//	if (MouseClick)
//	{
//		MouseClick = !MouseClick;
//		return !MouseClick;
//	}
//	return MouseClick;
//}