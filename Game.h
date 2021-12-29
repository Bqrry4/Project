#pragma once
//#include "TextureManager.h"
#include "Level.h"
#include "Menu.h"

class Game {
	static int screenWidth;
	static int screenHeigth;

	bool IsRunning : 1;
	SDL_Window* window;
	SDL_Renderer* render;

	Menu** menu;
	Level* level;

	Game() : IsRunning(false), window(nullptr), render(nullptr), level(nullptr), menu(nullptr)
	{}
	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;
	Game(Game&&) = delete;
	Game& operator=(Game&&) = delete;
public:
	~Game()
	{
		SDL_Log("%p Destructor", level);
		delete level;
		delete menu[0];
		delete menu[1];
		delete menu;
	}

	inline static Game& GetInstance() { static Game instance; return instance; } 	//Using Meyers's Singleton
	inline SDL_Renderer* GetRender() { return render; }
	inline bool Running() { return IsRunning; }
	inline static int ScreenWidth() { return screenWidth; }
	inline static int ScreenHeigth() { return screenHeigth; }

	void Init(const char* title, int x, int y, Uint32 flags);
	void Clean();
	void Quit();

	void Events();
	void Render();
	void Update();

	//Menues functions;
	void MainMenuLoop();
	void PuaseMenuLoop();
	void SwitchToMainLoop();

	//Level functions;
	void LoadLevel();
	void UnloadLevel();
};