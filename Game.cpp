#include "Game.h"
#include "TextureManager.h"
#include "Input.h"
#include "Menu.h"
#include "SystemTimer.h"


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

	menu = new Menu * [2];

	menu[0] = new MainMenu;
	menu[0]->SwitchTrigger();
	menu[1] = new PauseMenu;

	level = new Level;
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
}

void Game::Events()
{

	Input::GetInstance().Read();
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
	while (menu[0]->IsTriggered())
	{
		menu[0]->Update();
		menu[0]->Draw();
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

void Game::PuaseMenuLoop()
{
	if (Input::GetInstance().KeyState(SDL_SCANCODE_ESCAPE))
	{
		SDL_Log("Well2");
		menu[1]->SwitchTrigger();
	}

	while (menu[1]->IsTriggered())
	{
		SystemTimer::GetInstance()->Ticks();
		menu[1]->Update();
		menu[1]->Draw();
	}

}

void Game::SwitchToMainLoop()
{
	menu[0]->SwitchTrigger();
}

