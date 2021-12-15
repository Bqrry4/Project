#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <map>


enum Textures {
	Tex_Player,
	Tex_Dirt
};

class TextureManager {

	std::map<Uint16, SDL_Texture*> TextureMap;
	//std::vector<SDL_Texture*> TextureMap;
	TTF_Font* Font;

	TextureManager(): Font(nullptr)
	{}
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;
	TextureManager(TextureManager&&) = delete;
	TextureManager& operator=(TextureManager&&) = delete;

public:
	~TextureManager()
	{}

	inline static TextureManager& GetInstance() { static TextureManager instance; return instance; }

	bool Init();

	inline size_t TextureMapSize() { return TextureMap.size(); }
	bool Load(const char* path, Uint16 type);
	SDL_Texture* Load(const char* label, SDL_Color color, SDL_Rect dimension);
	SDL_Texture* Load(const char* path);

	SDL_Texture* FillTransparent(SDL_Texture* texture, SDL_Color color, SDL_Rect dimension);

	void Draw(Uint16 type, int x, int y, int w, int h, int row = 0, int frame = 0, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Point* CameraTranslate = nullptr);
	void Draw(SDL_Texture* texture, int x, int y, int w, int h, int row = 0, int frame = 0, SDL_RendererFlip flip = SDL_FLIP_NONE, SDL_Point* CameraTranslate = nullptr);
	
	void Clean();
	
};