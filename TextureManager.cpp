#include "TextureManager.h"
#include "Game.h"

bool TextureManager::Init()
{
	Font = TTF_OpenFont("assets/fonts/RobotoMono-Bold.ttf", 100);
	if (!Font)
	{
		SDL_Log("Failed to load font! %s", TTF_GetError());
		return false;
	}

	return true;
}

bool TextureManager::Load(const char* path, Uint16 type)
{
	if (path == nullptr)
	{
		Game::Log << "Invalid texture path";
		return false;
	}

	SDL_Surface* loadedSurface = IMG_Load(path);
	if (!loadedSurface)
	{
		Game::Log << "Failed loading image " << SDL_GetError() << '\n';
		return false;
	}

	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRender(), loadedSurface);
	if (!newTexture)
	{
		Game::Log << "Failed converting texture " << SDL_GetError() << '\n';
		return false;
	}
	SDL_FreeSurface(loadedSurface);

	TextureMap.push_back(newTexture);

	return true;
}

SDL_Texture* TextureManager::Load(const char* path, int* w, int* h)
{
	if (path == nullptr)
	{
		Game::Log << "Invalid texture path";
		return nullptr;
	}

	SDL_Surface* loadedSurface = IMG_Load(path);
	if (!loadedSurface)
	{
		Game::Log << "Failed loading image " << SDL_GetError() << '\n';
		return nullptr;
	}
	if (w != nullptr)
	{
		*w = loadedSurface->w;
	}
	if (h != nullptr)
	{
		*h = loadedSurface->h;
	}

	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRender(), loadedSurface);
	if (!newTexture)
	{
		Game::Log << "Failed converting texture " << SDL_GetError() << '\n';
		return nullptr;
	}
	SDL_FreeSurface(loadedSurface);

	return newTexture;
}

SDL_Texture* TextureManager::Load(const char* label, SDL_Color color, SDL_Rect dimension)
{
	if (label == nullptr)
	{
		Game::Log << "Invalid label to texture";
		return nullptr;
	}


	SDL_Surface* loadedSurface = TTF_RenderText_Solid(Font, label, color);
	if (!loadedSurface)
	{
		Game::Log << "Failed loading image  " << SDL_GetError() << '\n';
		return nullptr;
	}

	SDL_Point temp;
	temp.x = loadedSurface->w;
	temp.y = loadedSurface->h;



	SDL_Texture* Texture = SDL_CreateTextureFromSurface(Game::GetInstance().GetRender(), loadedSurface);
	if (!Texture)
	{
		Game::Log << "Failed converting texture " << SDL_GetError() << '\n';
		return nullptr;
	}
	SDL_FreeSurface(loadedSurface);

	SDL_Texture* newtexture = SDL_CreateTexture(Game::GetInstance().GetRender(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, dimension.w, dimension.h);
	SDL_SetTextureBlendMode(newtexture, SDL_BLENDMODE_BLEND);



	SDL_SetRenderTarget(Game::GetInstance().GetRender(), newtexture);
	SDL_SetRenderDrawColor(Game::GetInstance().GetRender(), color.r, color.g, color.b, 0);
	SDL_RenderClear(Game::GetInstance().GetRender());

	SDL_Rect dest = { dimension.w /2 - temp.x/2, dimension.h/2 - temp.y/2, temp.x, temp.y};
	SDL_RenderCopy(Game::GetInstance().GetRender(), Texture, NULL, &dest);

	SDL_DestroyTexture(Texture);
	SDL_SetRenderTarget(Game::GetInstance().GetRender(), NULL);

	return newtexture;
}

SDL_Texture* TextureManager::FillTransparent(SDL_Texture* texture, SDL_Color color, SDL_Rect dimension)
{
	SDL_Texture* newtexture = SDL_CreateTexture(Game::GetInstance().GetRender(), SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, dimension.w, dimension.h);
	
	SDL_SetRenderTarget(Game::GetInstance().GetRender(), newtexture);

	SDL_SetRenderDrawColor(Game::GetInstance().GetRender(), color.r, color.g, color.b, color.a);
	SDL_RenderClear(Game::GetInstance().GetRender());

	if (texture)
	{
		SDL_RenderCopy(Game::GetInstance().GetRender(), texture, NULL, NULL);
		SDL_DestroyTexture(texture);
	}

	SDL_SetRenderTarget(Game::GetInstance().GetRender(), NULL);

	return newtexture;

}

void TextureManager::Draw(Uint16 type, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip, const SDL_Point* CameraTranslate)
{
	if (CameraTranslate != nullptr)
	{
		destRect.x -= CameraTranslate->x;
		destRect.y -= CameraTranslate->y;
	}

	if (destRect.x + destRect.w > 0 && destRect.x < Game::ScreenWidth() && destRect.y + destRect.h > 0 && destRect.y < Game::ScreenHeigth()) //Render only if it is on the screen
	{
		SDL_Texture* texture;
		if (type >= TextureMap.size())
		{
			texture = nullptr;
		}
		else
		{
			texture = TextureMap[type];
		}

		SDL_RenderCopyEx(Game::GetInstance().GetRender(), texture, &srcRect, &destRect, 0, nullptr, flip);
	}
}

void TextureManager::Draw(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip, const SDL_Point* CameraTranslate)
{
	if (CameraTranslate != nullptr)
	{
		destRect.x -= CameraTranslate->x;
		destRect.y -= CameraTranslate->y;
	}
	if (destRect.x + destRect.w > 0 && destRect.x < Game::ScreenWidth() && destRect.y + destRect.h > 0 && destRect.y < Game::ScreenHeigth()) //Render only if it is on the screen
	{
		SDL_RenderCopyEx(Game::GetInstance().GetRender(), texture, &srcRect, &destRect, 0, nullptr, flip);
	}
}

void TextureManager::Clean()
{
	for (SDL_Texture* tex : TextureMap)
	{
		SDL_DestroyTexture(tex);
	}
	TextureMap.clear();

	TTF_CloseFont(Font);
	Font = NULL;
}