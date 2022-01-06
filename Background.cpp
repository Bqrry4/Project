#include "Background.h"
#include "Game.h"

bool Background::Parse(XMLElement* root, int iLayer)
{
	XMLElement* xmlElem = root->FirstChildElement("layer");

	xmlElem = root->FirstChildElement("layer");
	for (int i = 0; i < iLayer; ++i)
	{
		xmlElem = xmlElem->NextSiblingElement();
	}

	texture = TextureManager::Load(xmlElem->Attribute("texture"), &Width, &Height); // Check for nullptr
	xmlElem->QueryFloatAttribute("scrollratio", &Scrollratio);
	
	return true;
}

void Background::Draw(const SDL_Point* CameraTranslate)
{
	int ScaledWidth = (int)(Width * Game::ScreenHeigth() / Height) ;
	int ScaledHeight = Game::ScreenHeigth();

	int x = (int)(CameraTranslate->x * Scrollratio) % ScaledWidth;
	int y = 0; //red

	TextureManager::Draw(texture, { 0, 0, Width, Height }, { -x, y, ScaledWidth, ScaledHeight });
	TextureManager::Draw(texture, { 0, 0, Width, Height }, { -x + ScaledWidth, y, ScaledWidth, ScaledHeight });
}