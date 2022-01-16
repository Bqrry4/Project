//Started 15:30 , 13/11
#include "Game.h"
#include "SystemTimer.h"

//Screen dimension

int Game::screenWidth = 1024;
int Game::screenHeigth = 768;


int main(int argc, char* argv[])
{
	Game::GetInstance().Init("mY Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SDL_WINDOW_SHOWN);
	while (Game::GetInstance().Running())
	{
		Game::GetInstance().MainMenuLoop();
		Game::GetInstance().PuaseMenuLoop();
		Game::GetInstance().GameOverMenuLoop();
		Game::GetInstance().LevelClearMenuLoop();
		Game::GetInstance().Events();
		Game::GetInstance().Update();
		Game::GetInstance().Render();
		SystemTimer::GetInstance().Ticks();
	}

	Game::GetInstance().Clean();
	return 0;
}

//Rezolva cu destructorii!