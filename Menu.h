#pragma once
#include "Button.h"

class Menu
{
protected:
	Uint32 EventId;

	Button** buttons;
	__int8 buttonsCount;
	void DrawButtons();

	bool MenuTrigger;

	SDL_Point mouse;
	bool MouseClick;

	virtual void Events(SDL_Event*);
	virtual void ActionHandler(SDL_Event*) = 0;

public:
	Menu();
	virtual ~Menu();

	inline bool IsTriggered() { return MenuTrigger; }
	inline void SwitchTrigger() { MenuTrigger = !MenuTrigger; }

	virtual void Update() = 0;
	virtual void Draw() = 0;
};




class MainMenu : public Menu
{
	enum {
		Play,
		Exit,
		Level1,
		Level2,
		Level3,
		Easy,
		Normal,
		Hard,
		Swordsman,
		Archer,
		Return
	};
	SDL_Texture* Background;
	Music BackMusic;

	bool WasClosed;

	void ActionHandler(SDL_Event* event);
public:
	MainMenu();
	~MainMenu();

	void Update();
	void Draw();
};


class PauseMenu : public Menu
{
	enum {
		Resume,
		MainMenu,
		Exit
	};

	void ActionHandler(SDL_Event* event);

public:
	PauseMenu();
	~PauseMenu() = default;

	void Update();
	void Draw();

};

class GameOverMenu : public Menu
{
	enum {
		Replay,
		MainMenu
	};

	SDL_Texture* Label;

	void ActionHandler(SDL_Event*);

public:
	GameOverMenu();
	~GameOverMenu() = default;

	void Update();
	void Draw();

};

class LevelClearMenu : public Menu
{
	enum {
		NextLevel,
		Replay,
		MainMenu
	};

	SDL_Texture* Label;

	void ActionHandler(SDL_Event*);

public:
	LevelClearMenu();
	~LevelClearMenu() = default;

	void Update();
	void Draw();

};