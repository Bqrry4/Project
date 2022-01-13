#include "Game.h"
#include "TextureManager.h"
#include "Input.h"
#include "Menu.h"
#include "SystemTimer.h"


void Game::Init(const char *title, int x, int y, Uint32 flags)
{

	if (!SDL_Init(SDL_INIT_EVERYTHING) && IMG_Init(IMG_INIT_PNG) && !TTF_Init())
	{
		SDL_Log("System initialysed \n");
		window = SDL_CreateWindow(title, x, y, screenWidth, screenHeigth, flags);
		if (!window)
		{
			SDL_Log("Failed window creation %s \n", SDL_GetError());
			return;
		}
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

	srand(SDL_GetTicks() % 802); //Set randomizer seed

	menu = new Menu * [MenuesCount];

	menu[Main] = new MainMenu;
	menu[Main]->SwitchTrigger();
	menu[Pause] = new PauseMenu;
	menu[GameOver] = new GameOverMenu;
	menu[LevelClear] = new LevelClearMenu;

	//level = new Level;
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

	if(Level::IsLoaded)
	{
		level->Draw();
	}
	SDL_RenderPresent(render);
}
void Game::Update()
{
	if (Level::IsLoaded)
	{
		level->Update();
	}
}

void Game::MainMenuLoop()
{
	while (menu[Main]->IsTriggered())
	{
		menu[Main]->Update();
		menu[Main]->Draw();
	}
	if(Game::IsRunning && !Level::IsLoaded) LoadLevel();
}
void Game::PuaseMenuLoop()
{
	if (Input::GetInstance().KeyState(SDL_SCANCODE_ESCAPE))
	{
		menu[Pause]->SwitchTrigger();
	}

	while (menu[Pause]->IsTriggered())
	{
		SystemTimer::GetInstance()->Ticks();
		menu[Pause]->Update();
		menu[Pause]->Draw();
	}

}

void Game::GameOverMenuLoop()
{
	if (Level::IsLoaded && Player::PlayerDead)
	{
		menu[GameOver]->SwitchTrigger();
	}

	while (menu[GameOver]->IsTriggered())
	{
		menu[GameOver]->Update();
		menu[GameOver]->Draw();
	}
}

void Game::LevelClearMenuLoop()
{
	if (Level::IsLoaded && Boss::IsDefeated)
	{
		menu[LevelClear]->SwitchTrigger();
	}

	while (menu[LevelClear]->IsTriggered())
	{
		menu[LevelClear]->Update();
		menu[LevelClear]->Draw();
	}
}

void Game::SwitchToMainLoop()
{
	menu[Main]->SwitchTrigger();
}

void Game::LoadLevel()
{
	level = new Level;
	level->Load();
}
void Game::UnloadLevel()
{	
	delete level;
	level = nullptr;
	Level::IsLoaded = false;
	TextureManager::GetInstance().Clean();
}