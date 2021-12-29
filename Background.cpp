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
	int x = (int)(CameraTranslate->x * Scrollratio) % Width;
	int y = 0; //red

	TextureManager::Draw(texture, { 0, 0, Width, Height }, { -x, y, Width, Height }, SDL_FLIP_NONE);
	TextureManager::Draw(texture, { 0, 0, Width, Height }, { -x + Width, y, Width, Height }, SDL_FLIP_NONE);
	//TextureManager::GetInstance().Draw(texture, 0, 0, 3000, 1500, 0, 0, SDL_FLIP_NONE, CameraTranslate);
}