#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <map>


enum Textures {
	Tex_Player,
	Tex_Dirt
};

class TextureManager {

	static TextureManager* t_Instance;
	std::map<Uint16, SDL_Texture*> TextureMap;

	TextureManager()
	{}

public:
	~TextureManager()
	{
		delete t_Instance;
	}


	inline static TextureManager* GetInstance() { if (!t_Instance) t_Instance = new TextureManager; return t_Instance; } //Using Singleton

	inline size_t TextureMapSize() { return TextureMap.size(); }
	bool Load(const char* path, Uint16 type);
	void Draw(Uint16 type, int x, int y, int w, int h, int row = 0, int frame = 0, SDL_RendererFlip flip = SDL_FLIP_NONE); 
	void Clean();
	
};