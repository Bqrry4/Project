#include <iostream>
#include <string>
#include <sstream>
#include "TileLayer.h"
#include "Game.h"

bool TileLayer::Parse(XMLElement* root, int iLayer)
{
	root->QueryIntAttribute("tilewidth", &tileW);
	root->QueryIntAttribute("tileheight", &tileH);


	XMLElement* xmlElem = root->FirstChildElement("tileset");
	xmlElem->QueryIntAttribute("columns", &assetColumn);

	xmlElem = root->FirstChildElement("layer");
	for (int i = 0; i < iLayer; ++i)
	{
		xmlElem = xmlElem->NextSiblingElement();
	}

	xmlElem->QueryIntAttribute("width", &LayerWidth);
	xmlElem->QueryIntAttribute("height", &LayerHeigth);

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



void TileLayer::Draw()
{
	int row, column, Heigth = Game::GetInstance()->ScreenHeigth() / tileH, Width = Game::GetInstance()->ScreenWidth() / tileW;

	for (int id = 0; id < Heigth; ++id)
	{
		for (int jd = 0; jd < Width; ++jd)
		{
			if (TileIdMap[id][jd] == 0) continue;
			column = TileIdMap[id][jd] % assetColumn -1;
			row = TileIdMap[id][jd] / assetColumn;
			TextureManager::GetInstance()->Draw(0, offSetX + jd* tileW, offSetY + id * tileH, tileW, tileH, row, column);

		}
	}
}