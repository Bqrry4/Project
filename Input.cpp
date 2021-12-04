#include "Input.h"
#include "Game.h"

Input* Input::i_Instance = nullptr;

Input::Input()
{
	currentKeyStates = SDL_GetKeyboardState(NULL);
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