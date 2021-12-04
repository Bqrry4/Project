#include "TextureManager.h"
#include "Game.h"

TextureManager* TextureManager::t_Instance = nullptr;

bool TextureManager::Load(const char* path, Uint16 type)
{

	SDL_Surface* loadedSurface = IMG_Load(path);
	if (!loadedSurface)
	{
		SDL_Log("Failed loading image %s \n", SDL_GetError());
		return false;
	}

	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(Game::GetInstance()->GetRender(), loadedSurface);
	if (!newTexture)
	{
		SDL_Log("Failed converting texture %s \n", SDL_GetError());
		return false;
	}
	SDL_FreeSurface(loadedSurface);

	TextureMap[type] = newTexture;

	return true;
}

void TextureManager::Draw(Uint16 type, int x, int y, int w, int h, int row, int column, SDL_RendererFlip flip) //Pentru imagine statica, valorile {row, column} sunt implicite
{
	SDL_Rect srcRect = { w* column, h*row, w, h };
	SDL_Rect destRect = { x , y, w, h };
	SDL_RenderCopyEx(Game::GetInstance()->GetRender(), TextureMap[type], &srcRect, &destRect, 0, nullptr, flip);

}

void TextureManager::Clean()
{
	std::map<Uint16, SDL_Texture*>::iterator i;
	for (i = TextureMap.begin(); i != TextureMap.end(); ++i)
	{
		SDL_DestroyTexture(i->second);
	}
	TextureMap.clear();
}