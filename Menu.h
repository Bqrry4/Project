#pragma once
#include "Button.h"

class Menu
{
protected:
	Uint32 EventId;

	Button** buttons;
	__int8 buttonsCount;

	bool MenuTrigger;

	SDL_Point mouse;
	bool MouseClick;

public:
	Menu();
	virtual ~Menu() = default;

	inline bool IsTriggered() { return MenuTrigger; }
	inline void SwitchTrigger() { MenuTrigger = !MenuTrigger; }

	virtual void Events();
	virtual void ActionHandler(SDL_Event*) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
};




class MainMenu : public Menu
{
	SDL_Texture* Background;

public:
	MainMenu();
	~MainMenu();

	void ActionHandler(SDL_Event* event);
	void Update();
	void Draw();
};


class PauseMenu : public Menu
{
	int timeElapsed;

public:
	PauseMenu();
	~PauseMenu();

	void ActionHandler(SDL_Event* event);
	void Update();
	void Draw();

};