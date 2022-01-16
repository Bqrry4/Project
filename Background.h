#pragma once
#include "Layer.h"
class Background : public Layer
{
	float Scrollratio;

	int Width;
	int Height;

public:
	Background() : Layer(), Scrollratio(1.0f), Width(1), Height(1)
	{}
	~Background() = default;
	void Draw(const SDL_Point* CameraTranslate);
	bool Parse(XMLElement* root, int iLayer = 0);
};

