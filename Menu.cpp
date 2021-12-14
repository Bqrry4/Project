#include "Menu.h"
#include "Game.h"


Uint16 MainMenu::option1 = 0;
Uint16 MainMenu::option2 = 0;

MainMenu::MainMenu() : Menu()
{
	buttonsCount = 9;
	buttons = new Button * [buttonsCount];

	buttons[0] = new Button({ 262, 48, 500, 160 }, "SOLO", &Button1, true);
	buttons[1] = new Button({ 262, 304, 500, 160 }, "DUO", &Button2, true);
	buttons[2] = new Button({ 262, 560, 500, 160 }, "EXIT", &Button3, true);

	buttons[3] = new Button({ 262, 48, 500, 160 }, "Level1", &Button4, false);
	buttons[4] = new Button({ 262, 304, 500, 160 }, "Level2", &Button5, false);
	buttons[5] = new Button({ 262, 560, 500, 160 }, "Level3", &Button6, false);

	buttons[6] = new Button({ 262, 142, 500, 160 }, "Swordsman", &Button7, false);
	buttons[7] = new Button({ 262, 466, 500, 160 }, "Archer", &Button8, false);

	buttons[8] = new Button({ 890, 660, 80, 80 }, "<", &Button9, false);

	Background = TextureManager::GetInstance().Load("assets/menu_background.png");	//Check for missing file!!!!

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



void MainMenu::Update()
{
	for (int i = 0; i < buttonsCount; ++i)
	{
		buttons[i]->Update();
	}

	switch (option1)
	{
	case 0:
		break;
	case 1:
		for (int i = 0; i < 6; ++i)
			buttons[i]->SwitchActiveMode();
		buttons[8]->SwitchActiveMode();
		option1 = 0;
		break;
	case 2:
		for (int i = 3; i < 8; ++i)
			buttons[i]->SwitchActiveMode();
		option1 = 0;
		break;
	case 3:
		for (int i = 6; i < 8; ++i)
			buttons[i]->SwitchActiveMode();
		MenuTrigger = false;
		option1 = 0;
		break;
	case 4:
		buttons[8]->SwitchActiveMode();
		for (int i = 0; i < 3; ++i)
			buttons[i]->SwitchActiveMode();
		if (option2 == 1)
		{
			for (int i = 3; i < 6; ++i)
				buttons[i]->SwitchActiveMode();
		}
		if (option2 == 2)
		{
			for (int i = 6; i < 8; ++i)
				buttons[i]->SwitchActiveMode();
		}
		option1 = 0;
		break;
	}
}

void MainMenu::Draw()
{
	SDL_SetRenderDrawColor(Game::GetInstance().GetRender(), 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(Game::GetInstance().GetRender());

	TextureManager::GetInstance().Draw(Background, 0, 0, 1024, 768);
	for (int i = 0; i < buttonsCount; ++i)
	{
		buttons[i]->Draw();
	}

	SDL_RenderPresent(Game::GetInstance().GetRender());

}

#pragma region Buttons function

void MainMenu::Button1()
{
	Level::GameMode = false;
	option1 = 1;
	option2 = 1;
}	
void MainMenu::Button2()
{
	Level::GameMode = true;
	option1 = 1;
	option2 = 1;
}
void MainMenu::Button3()
{
	Game::GetInstance().Quit();
}

void MainMenu::Button4()
{
	Level::Levelid = 1;
	option1 = 2;
	option2 = 2;
}

void MainMenu::Button5()
{
	Level::Levelid = 2;
	option1 = 2;
	option2 = 2;
}

void MainMenu::Button6()
{
	Level::Levelid = 3;
	option1 = 2;
	option2 = 2;
}

void MainMenu::Button7()
{
	Level::PlayerClass = 1;
	option1 = 3;
}

void MainMenu::Button8()
{
	Level::PlayerClass = 2;
	option1 = 3;
}

void MainMenu::Button9()
{
	Level::GameMode = 0;
	Level::Levelid = 0;
	option1 = 4;
}

#pragma endregion