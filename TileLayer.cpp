#include <iostream>
#include <string>
#include <sstream>
#include "TileLayer.h"
#include "Game.h"

bool TileLayer::Parse(XMLElement* root, int iLayer)
{
	XMLElement* xmlElem = root->FirstChildElement("layer");

	xmlElem = root->FirstChildElement("layer");
	for (int i = 0; i < iLayer; ++i)
	{
		xmlElem = xmlElem->NextSiblingElement();
	}

	texture = TextureManager::Load(xmlElem->Attribute("texture")); // Check for nullptr
	xmlElem->QueryIntAttribute("columns", &assetColumn);
	xmlElem->QueryIntAttribute("width", &LayerWidth);
	xmlElem->QueryIntAttribute("height", &LayerHeigth);
	xmlElem->QueryIntAttribute("tilewidth", &tileW);
	xmlElem->QueryIntAttribute("tileheight", &tileH);

	XMLElement* xmlData = xmlElem->FirstChildElement("data");
	std::string matrix(xmlData->GetText());
	std::istringstream stream(matrix);
	std::string value;

	TileIdMap = new Uint16*[LayerHeigth];
	for (int id = 0; id < LayerHeigth; ++id)
	{
		TileIdMap[id] = new Uint16[LayerWidth];
		for (int jd = 0; jd < LayerWidth; ++jd)
		{
			std::getline(stream, value, ',');
			std::stringstream convertor(value);
			convertor >> TileIdMap[id][jd];
			if (!stream.good()) break;
		}
	}
	return true;		
}



void TileLayer::Draw(const SDL_Point* CameraTranslate)
{
	int row, column, Heigth = Game::ScreenHeigth() / tileH, Width = Game::ScreenWidth() / tileW;

	int id = CameraTranslate->y / tileH;

	Heigth = ((Heigth + id + 2) > LayerHeigth) ? (LayerHeigth) : (Heigth + id + 2);
	for (id; id < Heigth; ++id)
	{
		int jd = CameraTranslate->x / tileW -1;

		int Width_d = ((Width + jd +2) > LayerWidth) ? (LayerWidth) : (Width + jd + 2);
		for (jd; jd < Width_d; ++jd)
		{
			if (TileIdMap[id][jd] == 0) continue;
			column = TileIdMap[id][jd] % assetColumn -1;
			row = TileIdMap[id][jd] / assetColumn;
			TextureManager::Draw(texture, { tileW * column, tileH * row, tileW, tileH }, { offSetX + jd * tileW, offSetY + id * tileH, tileW, tileH }, SDL_FLIP_NONE, CameraTranslate);
		}
	}
}