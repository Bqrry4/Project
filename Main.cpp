//Started 15:30 , 13/11
#include "Game.h"
#include "SystemTimer.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

int main(int argc, char* argv[])
{
	Game::GetInstance().Init("mY Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	while (Game::GetInstance().Running())
	{
		Game::GetInstance().MainMenuLoop();
		Game::GetInstance().PuaseMenu();
		Game::GetInstance().Events();
		Game::GetInstance().Update();
		Game::GetInstance().Render();
		SystemTimer::GetInstance()->Ticks();
	}

	Game::GetInstance().Clean();
	return 0;
}

//Rezolva cu destructorii!