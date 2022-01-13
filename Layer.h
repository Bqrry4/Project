#pragma once
#include "Vendor/tinyxml2.h"
using namespace tinyxml2;
#include "TextureManager.h"


class Layer {
protected:
	SDL_Texture* texture;

public:
	Layer() : texture(nullptr)
	{}
	virtual ~Layer()
	{
		SDL_DestroyTexture(texture);
	}

	virtual void Draw(const SDL_Point* CameraTranslate) = 0;
	virtual bool Parse(XMLElement* root, int iLayer = 0) = 0;

};