#pragma once
//#include "TextureManager.h"
#include "Level.h"
#include "Menu.h"

class Game {

	bool IsRunning : 1;
	SDL_Window* window;
	SDL_Renderer* render;

	Game() : IsRunning(false), window(nullptr), render(nullptr), level(nullptr), menu(nullptr), screenWidth(0), screenHeigth(0)
	{}
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;


	Menu* menu;

	Level* level;

	int screenWidth;
	int screenHeigth;

public:
	~Game()
	{
		delete level;
		delete menu;
	}

	inline static Game& GetInstance() { static Game instance; return instance; } 	//Using Meyers's Singleton
	inline SDL_Renderer* GetRender() { return render; }
	inline bool Running() { return IsRunning; }
	inline int ScreenWidth() { return screenWidth; }
	inline int ScreenHeigth() { return screenHeigth; }


	void Init(const char* title, int x, int y, int w, int h, Uint32 flags);
	void Clean();
	void Quit();

	void Events();
	void Render();
	void Update();

	//Menues functions;

	void MainMenuLoop();
	void PuaseMenu();



};