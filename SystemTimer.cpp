#include "SystemTimer.h"
#include <SDL.h>
SystemTimer* SystemTimer::st_Instance = nullptr;

void SystemTimer::Ticks()
{
	delta_time = (SDL_GetTicks() - last_time) / 1000.0f;
	//if (delta_time > 2.5)
	//	delta_time = 2.5;


	last_time = SDL_GetTicks();
}