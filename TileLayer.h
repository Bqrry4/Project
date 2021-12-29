#pragma once
#include "Layer.h"

class TileLayer : public Layer
{
	int LayerWidth;
	int LayerHeigth;

	int tileW;
	int tileH;

	Uint16** TileIdMap;

	int offSetX;
	int offSetY;

	int assetColumn;


	//friend class Level;
public:
	TileLayer() : Layer(), LayerWidth(0), LayerHeigth(0), tileW(0), tileH(0), TileIdMap(nullptr), offSetX(0), offSetY(0), assetColumn(0)
	{}
	~TileLayer()
	{
		for (int i = 0; i < LayerHeigth; ++i) delete[]TileIdMap[i];
		delete[]TileIdMap;
	}
	//virtual void Update();
	void Draw(const SDL_Point* CameraTranslate = nullptr);
 	bool Parse(XMLElement* root, int iLayer = 0);

	inline Uint16 GetGrid(Uint16 i, Uint16 j) { if (i >= LayerHeigth || j >= LayerWidth) return 0; return TileIdMap[i][j]; }
	inline int GetGridW() { return tileW; }
	inline int GetGridH() { return tileH; }
};