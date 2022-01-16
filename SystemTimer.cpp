#include "SystemTimer.h"
#include <SDL.h>

void SystemTimer::Ticks()
{
	delta_time = (SDL_GetTicks() - last_time) / 1000.0f;
	if (delta_time > 0.04) delta_time = 0.04;

	last_time = SDL_GetTicks();
}