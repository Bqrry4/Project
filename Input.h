#pragma once
#include <SDL.h>

class Input {
	static Input* i_Instance;

	const Uint8* currentKeyStates;
	Input();
public:
	~Input()
	{
		delete i_Instance;
	}

	inline static Input* GetInstance() { if (!i_Instance) i_Instance = new Input; return i_Instance; } //Using Singleton

	void Read();
	void UpdateKeyStates();

	bool KeyState(SDL_Scancode key);

};