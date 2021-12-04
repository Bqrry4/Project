#pragma once
//#include "TextureManager.h"
#include "Level.h"


class Game {
	static Game* g_Instance;
	bool IsRunning;
	SDL_Window* window;
	SDL_Renderer* render;

	Game() : IsRunning(false), window(nullptr), render(nullptr), level(nullptr), screenWidth(0), screenHeigth(0)
	{}
	Game(const Game&);
	Game& operator=(const Game&) {}

	Level* level;

	int screenWidth;
	int screenHeigth;

public:
	~Game()
	{
		SDL_Log("!des");
		delete level;
		delete g_Instance;
		g_Instance = nullptr;
	}

	inline static Game* GetInstance() { if (!g_Instance) g_Instance = new Game; return g_Instance; } //Using Singleton
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


};


