#include "Game.h"
#include "TextureManager.h"
#include "Input.h"



Game* Game::g_Instance = nullptr;

void Game::Init(const char *title, int x, int y, int w, int h, Uint32 flags)
{

	if (!SDL_Init(SDL_INIT_EVERYTHING) && IMG_Init(IMG_INIT_PNG))
	{
		SDL_Log("System initialysed \n");
		window = SDL_CreateWindow(title, x, y, w, h, flags);
		if (!window)
		{
			SDL_Log("Failed window creation %s \n", SDL_GetError());
			return;
		}
		screenWidth = w;
		screenHeigth = h;
		
		render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!render)
		{
			SDL_Log("Failed render creation %s \n", SDL_GetError());
			return;
		}
		IsRunning = true;
	}
	else
	{
		SDL_Log("SDL failed initialisation \n");
		return;
	}

	TextureManager::GetInstance()->Load("assets/tiles.png", 0);
	level = new Level;
	level->LvLparser("assets/map1.tmx");

}

void Game::Clean()
{
	TextureManager::GetInstance()->Clean();
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	SDL_Log("System cleaned, quiting...");
}

void Game::Quit()
{
	IsRunning = false;
	
}

void Game::Events()
{

	Input::GetInstance()->Read();
}

void Game::Render()
{
	SDL_SetRenderDrawColor(render, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(render);

	level->Draw();

	SDL_RenderPresent(render);
}

void Game::Update()
{
	level->Update();
}

