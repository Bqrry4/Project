#pragma once
#include "Button.h"

class Menu
{
protected:
	Button** buttons;
	__int8 buttonsCount;

	bool MenuTrigger;

public:
	inline bool IsTriggered() { return MenuTrigger; }
	inline void SwitchTrigger() { MenuTrigger = !MenuTrigger; }

	Menu() : buttons(nullptr), buttonsCount(0), MenuTrigger(false)
	{}
	virtual ~Menu() = default;

	virtual void Update() = 0;
	virtual void Draw() = 0;
};




class MainMenu : public Menu
{
	SDL_Texture* Background;

	static Uint16 option1;
	static Uint16 option2;

public:
	MainMenu();
	~MainMenu();

	void Update();
	void Draw();

	static void Button1();
	static void Button2();
	static void Button3();
	static void Button4();
	static void Button5();
	static void Button6();
	static void Button7();
	static void Button8();
	static void Button9();

};
