#include "Menu.h"
#include "Game.h"
#include "SystemTimer.h"

Menu::Menu() : buttons(nullptr), buttonsCount(0), MenuTrigger(false), mouse({0,0}), MouseClick(false)
{
	EventId = SDL_RegisterEvents(1); //Verifica
	if (EventId == ((Uint32)-1))
	{
		SDL_Log("Error event register");
	}
}


MainMenu::MainMenu() : Menu()
{
	buttonsCount = 9;
	buttons = new Button * [buttonsCount];

	buttons[0] = new Button(1, { 262, 48, 500, 160 }, "SOLO", true, EventId);
	buttons[1] = new Button(2, { 262, 304, 500, 160 }, "DUO", true, EventId);
	buttons[2] = new Button(3, { 262, 560, 500, 160 }, "EXIT", true, EventId);

	buttons[3] = new Button(4, { 262, 48, 500, 160 }, "Level1", false, EventId);
	buttons[4] = new Button(5, { 262, 304, 500, 160 }, "Level2", false, EventId);
	buttons[5] = new Button(6, { 262, 560, 500, 160 }, "Level3", false, EventId);

	buttons[6] = new Button(7, { 262, 142, 500, 160 }, "Swordsman", false, EventId);
	buttons[7] = new Button(8, { 262, 466, 500, 160 }, "Archer", false, EventId);

	buttons[8] = new Button(9, { 890, 660, 80, 80 }, "<", false, EventId);

	Background = TextureManager::Load("assets/menu_background.png");	//Check for missing file!!!!

}

MainMenu::~MainMenu()
{
	SDL_DestroyTexture(Background);
	for (int i = 0; i < buttonsCount; ++i)
	{
		delete buttons[i];
	}
	delete buttons;
}

void Menu::Events()
{
	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			Game::GetInstance().Quit();
			SwitchTrigger();
			break;
		case SDL_MOUSEBUTTONUP:
			MouseClick = true;
			break;
		case SDL_MOUSEBUTTONDOWN:
			MouseClick = false;
			break;
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&mouse.x, &mouse.y);
			break;
		default:
			if (event.type == EventId)
				ActionHandler(&event);
			break;
		}
	}
}

void MainMenu::Update()
{
	Events();

	for (int i = 0; i < buttonsCount; ++i)
	{
		buttons[i]->CheckForPress(&mouse, MouseClick);
	}
	MouseClick = false;
}

void MainMenu::ActionHandler(SDL_Event* event)
{
	switch (event->user.code) //Buttons functionality
	{
	case 1:
		Level::GameMode = false;
		for (int i = 0; i < 6; ++i)
			buttons[i]->SwitchActiveMode();
		buttons[8]->SetActiveMode(true);
		break;
	case 2:
		Level::GameMode = true;
		for (int i = 0; i < 6; ++i)
			buttons[i]->SwitchActiveMode();
		buttons[8]->SetActiveMode(true);
		break;
	case 3:
		Game::GetInstance().Quit();
		SwitchTrigger();
		break;
	case 4:
		Level::Levelid = 1;
		for (int i = 3; i < 8; ++i)
			buttons[i]->SwitchActiveMode();
		break;
	case 5:
		Level::Levelid = 2;
		for (int i = 3; i < 8; ++i)
			buttons[i]->SwitchActiveMode();
		break;
	case 6:
		Level::Levelid = 3;
		for (int i = 3; i < 8; ++i)
			buttons[i]->SwitchActiveMode();
		break;
	case 7:
		Level::PlayerClass = PlayerClasses::Swordswman;
		for (int i = 6; i < 8; ++i)
			buttons[i]->SwitchActiveMode();
		for (int i = 0; i < 3; ++i)
			buttons[i]->SwitchActiveMode();
		buttons[8]->SetActiveMode(false);
		SwitchTrigger();
		break;
	case 8:
		Level::PlayerClass = PlayerClasses::Archer;
		for (int i = 6; i < 8; ++i)
			buttons[i]->SwitchActiveMode();
		for (int i = 0; i < 3; ++i)
			buttons[i]->SwitchActiveMode();
		buttons[8]->SetActiveMode(false);
		SwitchTrigger();
		break;
	case 9:
		for (int i = 3; i < 8; i++)
			buttons[i]->SetActiveMode(false);
		for (int i = 0; i < 3; ++i)
			buttons[i]->SetActiveMode(true);
		buttons[8]->SetActiveMode(false);
		break;
	}

}

void MainMenu::Draw()
{
	SDL_SetRenderDrawColor(Game::GetInstance().GetRender(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(Game::GetInstance().GetRender());

	TextureManager::Draw(Background, { 0, 0, 1024, 768 }, { 0, 0, 1024, 768 });
	for (int i = 0; i < buttonsCount; ++i)
	{
		buttons[i]->Draw();
	}

	SDL_RenderPresent(Game::GetInstance().GetRender());

}



PauseMenu::PauseMenu() : Menu(), timeElapsed(0)
{
	buttonsCount = 3;
	buttons = new Button * [buttonsCount];

	buttons[0] = new Button(1, { 262, 48, 500, 160 }, "Resume", true, EventId);
	buttons[1] = new Button(2, { 262, 304, 500, 160 }, "MainMenu", true, EventId);
	buttons[2] = new Button(3, { 262, 560, 500, 160 }, "Exit", true, EventId);

}

PauseMenu::~PauseMenu()
{
	for (int i = 0; i < buttonsCount; ++i)
	{
		delete buttons[i];
	}
	delete buttons;
}

void PauseMenu::Update()
{
	Events();

	if (timeElapsed < 500)
	{
		timeElapsed += SystemTimer::GetInstance()->GetDt() * 1000;
	}
	else
	{ 
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_ESCAPE])
		{
			SDL_Log("Well3");
			SwitchTrigger();
			timeElapsed = 0;
		}
	}

	for (int i = 0; i < buttonsCount; ++i)
	{
		buttons[i]->CheckForPress(&mouse, MouseClick);
	}
	MouseClick = false;
}

void PauseMenu::ActionHandler(SDL_Event* event)
{
	switch (event->user.code) //Buttons functionality
	{
	case 1:
		SwitchTrigger();
		break;
	case 2:
		Game::GetInstance().SwitchToMainLoop();
		Game::GetInstance().UnloadLevel();
		SwitchTrigger();
		break;
	case 3:
		Game::GetInstance().Quit();
		SwitchTrigger(); 
		break;
	}

}

void PauseMenu::Draw()
{
	//SDL_SetRenderDrawColor(Game::GetInstance().GetRender(), 0xFF, 0xFF, 0xFF, 0xFF);
	//SDL_RenderClear(Game::GetInstance().GetRender());
	//TextureManager::GetInstance().Draw(Background, 0, 0, 1024, 768);
	for (int i = 0; i < buttonsCount; ++i)
	{
		buttons[i]->Draw();
	}

	SDL_RenderPresent(Game::GetInstance().GetRender());

}
