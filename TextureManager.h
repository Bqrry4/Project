#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <vector>


class TextureManager {

	//std::map<Uint16, SDL_Texture*> TextureMap;
	std::vector<SDL_Texture*> TextureMap;
	TTF_Font* Font;

	TextureManager(): Font(nullptr)
	{}
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
	TextureManager(TextureManager&&) = delete;
	TextureManager& operator=(TextureManager&&) = delete;

public:
	~TextureManager() { Clean(); }

	inline static TextureManager& GetInstance() { static TextureManager instance; return instance; }

	bool Init();

	inline size_t TextureMapSize() { return TextureMap.size(); }

	bool Load(const char* path, Uint16 type);
	void Draw(Uint16 type, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip = SDL_FLIP_NONE, const SDL_Point* CameraTranslate = nullptr);

	SDL_Texture* Load(const char* label, SDL_Color color, SDL_Rect dimension);

	static SDL_Texture* Load(const char* path, int* w = nullptr, int* h = nullptr);
	static SDL_Texture* FillTransparent(SDL_Texture* texture, SDL_Color color, SDL_Rect dimension);
	static void Draw(SDL_Texture* texture, SDL_Rect srcRect, SDL_Rect destRect, SDL_RendererFlip flip = SDL_FLIP_NONE, const SDL_Point* CameraTranslate = nullptr);
	
	void Clean();
	
};