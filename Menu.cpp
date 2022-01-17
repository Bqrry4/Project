#include "Menu.h"
#include "Game.h"
#include "SystemTimer.h"

Menu::Menu() : buttons(nullptr), buttonsCount(0), MenuTrigger(false), mouse({0,0}), MouseClick(false)
{
	EventId = SDL_RegisterEvents(1);
	if (EventId == ((Uint32)-1))
	{
		SDL_Log("Error event register"); //Throw
	}
}
Menu::~Menu()
{
	for (int i = 0; i < buttonsCount; ++i)
	{
		delete buttons[i];
	}
	delete buttons;
}
void Menu::Events(SDL_Event* event)
{
	while (SDL_PollEvent(event) != 0)
	{
		switch (event->type)
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
			if (event->type == EventId)
				ActionHandler(event);
			break;
		}
	}
}
void Menu::DrawButtons()
{
	for (int i = 0; i < buttonsCount; ++i)
	{
		buttons[i]->Draw();
	}
}


MainMenu::MainMenu() : Menu(), WasClosed(false)
{
	buttonsCount = 11;
	buttons = new Button * [buttonsCount];

	buttons[Play] = new Button(Play, { (Game::ScreenWidth() - 540) / 2, (Game::ScreenHeigth() - 160)/2, 540, 160 }, "Play", true, EventId);
	buttons[Exit] = new Button(Exit, { (Game::ScreenWidth() - 340), (Game::ScreenHeigth() - 160), 340, 160 }, "Exit", true, EventId);

	buttons[Level1] = new Button(Level1, { (Game::ScreenWidth() - 540) / 2, 48, 540, 160 }, "Level1", false, EventId);
	buttons[Level2] = new Button(Level2, { (Game::ScreenWidth() - 540) / 2, 304, 540, 160 }, "Level2", false, EventId);
	buttons[Level3] = new Button(Level3, { (Game::ScreenWidth() - 540) / 2, 560, 540, 160 }, "Level3", false, EventId);

	buttons[Easy] = new Button(Easy, { (Game::ScreenWidth() - 540) / 2, 48, 540, 160 }, "Easy", false, EventId);
	buttons[Normal] = new Button(Normal, { (Game::ScreenWidth() - 540) / 2, 304, 540, 160 }, "Normal", false, EventId);
	buttons[Hard] = new Button(Hard, { (Game::ScreenWidth() - 540) / 2, 560, 540, 160 }, "Hard", false, EventId);

	buttons[Swordsman] = new Button(Swordsman, { (Game::ScreenWidth() - 540) / 2, 142, 540, 160 }, "Swordsman", false, EventId);
	buttons[Archer] = new Button(Archer, { (Game::ScreenWidth() - 540) / 2, 466, 540, 160 }, "Archer", false, EventId);

	buttons[Return] = new Button(Return, { (Game::ScreenWidth() - 80), (Game::ScreenHeigth() - 80), 80, 80 }, "<", false, EventId);

	Background = TextureManager::Load("assets/textures/menu_background.png");

	BackMusic.Load("assets/sounds/menu.ogg");
	BackMusic.Play();
}
MainMenu::~MainMenu()
{
	SDL_DestroyTexture(Background);
}
void MainMenu::Update()
{
	if (WasClosed)
	{
		WasClosed = false;
		buttons[Play]->SetActiveMode(true);
		buttons[Exit]->SetActiveMode(true);
		BackMusic.Play();
	}
	SDL_Event event;
	Events(&event);

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
	case Play:
		for (int i = 0; i < 5; ++i)
			buttons[i]->SwitchActiveMode();
		buttons[Return]->SetActiveMode(true);
		break;
	case Exit:
		Game::GetInstance().Quit();
		SwitchTrigger();
		break;
	case Level1:
		Level::Levelid = 1;
		for (int i = 2; i < 8; ++i)
			buttons[i]->SwitchActiveMode();
		break;
	case Level2:
		Level::Levelid = 2;
		for (int i = 2; i < 8; ++i)
			buttons[i]->SwitchActiveMode();
		break;
	case Level3:
		Level::Levelid = 3;
		for (int i = 2; i < 8; ++i)
			buttons[i]->SwitchActiveMode();
		break;
	case Easy:
		Level::DifficultyAmpl = 0.75;
		for (int i = 5; i < 10; ++i)
			buttons[i]->SwitchActiveMode();
		break;
	case Normal:
		Level::DifficultyAmpl = 1.f;
		for (int i = 5; i < 10; ++i)
			buttons[i]->SwitchActiveMode();
		break;
	case Hard:
		Level::DifficultyAmpl = 1.25;
		for (int i = 5; i < 10; ++i)
			buttons[i]->SwitchActiveMode();
		break;
	case Swordsman:
		Level::PlayerClass = PlayerClasses::Swordswman;
		for (int i = 8; i < 11; ++i)
			buttons[i]->SwitchActiveMode();
		WasClosed = true;
		SwitchTrigger();
		break;
	case Archer:
		Level::PlayerClass = PlayerClasses::Archer;
		for (int i = 8; i < 11; ++i)
			buttons[i]->SwitchActiveMode();
		WasClosed = true;
		SwitchTrigger();
		break;
	case Return:
		for (int i = 0; i < 2; ++i)
			buttons[i]->SetActiveMode(true);
		for (int i = 2; i < 10; i++)
			buttons[i]->SetActiveMode(false);
		buttons[Return]->SetActiveMode(false);
		break;
	}

}
void MainMenu::Draw()
{
	SDL_SetRenderDrawColor(Game::GetInstance().GetRender(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(Game::GetInstance().GetRender());

	TextureManager::Draw(Background, { 0, 0, Game::ScreenWidth(), Game::ScreenHeigth() }, { 0, 0, Game::ScreenWidth(), Game::ScreenHeigth() });
	DrawButtons();

	SDL_RenderPresent(Game::GetInstance().GetRender());
}


PauseMenu::PauseMenu() : Menu()
{
	buttonsCount = 3;
	buttons = new Button * [buttonsCount];

	buttons[Resume] = new Button(Resume, { (Game::ScreenWidth() - 500)/2, 48, 500, 160 }, "Resume", true, EventId);
	buttons[MainMenu] = new Button(MainMenu, { (Game::ScreenWidth() - 500) / 2, 304, 500, 160 }, "MainMenu", true, EventId);
	buttons[Exit] = new Button(Exit, { (Game::ScreenWidth() - 500) / 2, 560, 500, 160 }, "Exit", true, EventId);
}
void PauseMenu::Update()
{
	SDL_Event event;
	Events(&event);

	if (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
	{
		SwitchTrigger();
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
	case Resume:
		SwitchTrigger();
		break;
	case MainMenu:
		Game::GetInstance().SwitchToMainLoop();
		Game::GetInstance().UnloadLevel();
		SwitchTrigger();
		break;
	case Exit:
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
	DrawButtons();

	SDL_RenderPresent(Game::GetInstance().GetRender());
}


GameOverMenu::GameOverMenu() : Menu()
{
	Label = TextureManager::GetInstance().Load("GameOver", { 0xFF, 0, 0, 0xFF }, { 0, 0, 500, 160 }); //Check for nullptr
	
	buttonsCount = 2;
	buttons = new Button * [buttonsCount];

	buttons[Replay] = new Button(Replay, { (Game::ScreenWidth() - 500) / 2, 234, 500, 160 }, "Replay", true, EventId);
	buttons[MainMenu] = new Button(MainMenu, { (Game::ScreenWidth() - 500) / 2, 490, 500, 160 }, "MainMenu", true, EventId);
}
void GameOverMenu::ActionHandler(SDL_Event* event)
{
	switch (event->user.code) //Buttons functionality
	{
	case Replay:
		Game::GetInstance().UnloadLevel();
		Game::GetInstance().LoadLevel();
		SwitchTrigger();
		break;
	case MainMenu:
		Game::GetInstance().SwitchToMainLoop();
		Game::GetInstance().UnloadLevel();
		SwitchTrigger();
		break;
	}

}
void GameOverMenu::Update()
{
	SDL_Event event;
	Events(&event);

	for (int i = 0; i < buttonsCount; ++i)
	{
		buttons[i]->CheckForPress(&mouse, MouseClick);
	}
	MouseClick = false;
}
void GameOverMenu::Draw()
{
	DrawButtons();

	TextureManager::Draw(Label, { 0, 0, 500, 160 }, { (Game::ScreenWidth() - 500) / 2, 28, 500, 160 });
	SDL_RenderPresent(Game::GetInstance().GetRender());
}


LevelClearMenu::LevelClearMenu() : Menu()
{
	Label = TextureManager::GetInstance().Load("LevelCleared", { 0, 0xFF, 0, 0xFF }, { 0, 0, 800, 100 }); //Check for nullptr

	buttonsCount = 3;
	buttons = new Button * [buttonsCount];

	buttons[NextLevel] = new Button(NextLevel, { (Game::ScreenWidth() - 540) / 2, 188, 540, 140 }, "NextLevel", true, EventId);
	buttons[Replay] = new Button(Replay, { (Game::ScreenWidth() - 540) / 2, 384, 540, 140 }, "Reply", true, EventId);
	buttons[MainMenu] = new Button(MainMenu, { (Game::ScreenWidth() - 540) / 2, 580, 540, 140 }, "MainMenu", true, EventId);
}
void LevelClearMenu::ActionHandler(SDL_Event* event)
{
	switch (event->user.code) //Buttons functionality
	{
	case NextLevel:
		Game::GetInstance().UnloadLevel();
		++Level::Levelid;						// verify Count of levels!!
		Game::GetInstance().LoadLevel();
		break;
	case Replay:
		Game::GetInstance().UnloadLevel();
		Game::GetInstance().LoadLevel();
		SwitchTrigger();
		break;
	case MainMenu:
		Game::GetInstance().SwitchToMainLoop();
		Game::GetInstance().UnloadLevel();
		SwitchTrigger();
		break;
	}
}
void LevelClearMenu::Update()
{
	SDL_Event event;
	Events(&event);

	for (int i = 0; i < buttonsCount; ++i)
	{
		buttons[i]->CheckForPress(&mouse, MouseClick);
	}
	MouseClick = false;
}
void LevelClearMenu::Draw()
{
	DrawButtons();

	TextureManager::Draw(Label, { 0, 0, 800, 100 }, { (Game::ScreenWidth() - 800) / 2, 28, 800, 100 });
	SDL_RenderPresent(Game::GetInstance().GetRender());
}