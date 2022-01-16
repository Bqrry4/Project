#pragma once
#include <SDL.h>

class Input {

	SDL_Event event;

	const Uint8* currentKeyStates;
	//SDL_Point mouse;

	//bool MouseClick;

	Input();
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;
	Input(Input&&) = delete;
	Input& operator=(Input&&) = delete;

	void UpdateKeyStates();
public:
	~Input() = default;

	inline static Input& GetInstance() { static Input instance; return instance; }

	void Read();

	bool WasPressed(SDL_Scancode);
	bool KeyState(SDL_Scancode key);

	//bool MouseClicked();
	//SDL_Point* MousePosition();

};