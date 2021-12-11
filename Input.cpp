#include "Input.h"
#include "Game.h"

Input* Input::i_Instance = nullptr;

Input::Input() : MouseClick(false)
{
	currentKeyStates = SDL_GetKeyboardState(NULL);
	SDL_GetMouseState(&mouse.x, &mouse.y);
}

void Input::Read()
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		Game::GetInstance()->Quit();
		break;
	case SDL_KEYDOWN:
		UpdateKeyStates();
		break;
	case SDL_KEYUP:
		UpdateKeyStates();
		break;
	case SDL_MOUSEBUTTONUP:
		MouseClick = false;
		break;
	case SDL_MOUSEBUTTONDOWN:
		MouseClick = true;
		break;
	case SDL_MOUSEMOTION:
		SDL_GetMouseState(&mouse.x, &mouse.y);
		break;

	}
}

void Input::UpdateKeyStates()
{
	currentKeyStates = SDL_GetKeyboardState(NULL);
}


bool Input::KeyState(SDL_Scancode key)
{
	return currentKeyStates[key];
}

SDL_Point* Input::MousePosition()
{
	return &mouse;
}