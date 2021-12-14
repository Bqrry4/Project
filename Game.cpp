#include "Game.h"
#include "TextureManager.h"
#include "Input.h"
#include "Menu.h"


void Game::Init(const char *title, int x, int y, int w, int h, Uint32 flags)
{

	if (!SDL_Init(SDL_INIT_EVERYTHING) && IMG_Init(IMG_INIT_PNG) && !TTF_Init())
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
		if (!TextureManager::GetInstance().Init())
		{
			SDL_Log("Failed to init TextureManager, see for missing files");
			return;
		}
		IsRunning = true;
	}
	else
	{
		SDL_Log("SDL failed initialisation \n");
		return;
	}


	//menu = new Menu((SDL_Texture*) nullptr, buttons);
	menu = new MainMenu;
	menu->SwitchTrigger();

	TextureManager::GetInstance().Load("assets/tiles.png", 0);
	level = new Level;
	//level->LvLparser("assets/map1.tmx");
}

void Game::Clean()
{
	TextureManager::GetInstance().Clean();
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	SDL_Log("System cleaned, quiting...");
}

void Game::Quit()
{
	IsRunning = false;
	if (menu->IsTriggered())
	{
		menu->SwitchTrigger();
	}
	
}

void Game::Events()
{

	Input::GetInstance()->Read();
}

void Game::Render()
{
	SDL_SetRenderDrawColor(render, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(render);

	if(Level::Levelid)
	{
		level->Draw();
	}
	SDL_RenderPresent(render);
}

void Game::Update()
{
	if (Level::Levelid)
	{
		level->Update();
	}
}

void Game::MainMenuLoop()
{
	while (menu->IsTriggered())
	{
		Input::GetInstance()->Read();
		menu->Update();
		menu->Draw();
	}

	if (!Level::IsLoaded)
	{
		switch (Level::Levelid)
		{
		case 0:
			break;
		case 1:
			level->LvLparser("assets/map1.tmx");
			break;
		case 2:
			level->LvLparser("assets/level2.xml");
			break;
		case 3:
			level->LvLparser("assets/level3.xml");
			break;
		default:
			SDL_Log("Something went wrong, level cannot be loaded...");
			break;
		}
	}
}

void Game::PuaseMenu()
{
	SDL_Log("bla bla");
}

