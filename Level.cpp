#include "Vendor/tinyxml2.h"
#include "Level.h"

using namespace tinyxml2;


bool Level::LvLparser(const char* path)
{
	XMLDocument lvlDocument;


	if (lvlDocument.LoadFile(path))  ///ERROR HANDLING DONT FORGET!
	{
		SDL_Log("Loading XML file error, %d", lvlDocument.ErrorID());
		return false;
	}

	XMLElement* root = lvlDocument.RootElement();   
	

	XMLElement* mapRoot = root->FirstChildElement("map");

	mapRoot->QueryIntAttribute("width", &mapHeigth);
	mapRoot->QueryIntAttribute("height", &mapWidth);


	XMLElement* xmlElem = mapRoot->FirstChildElement("layer");
	for (TileLayerCount; xmlElem != NULL; ++TileLayerCount, xmlElem = xmlElem->NextSiblingElement());
	
	layers = new TileLayer[TileLayerCount];


	//parsing TileLayers
	
	for (int i = 0; i < TileLayerCount; ++i)
	{
		if (!layers[i].Parse(mapRoot, i))
		{
			return false;
		}
	}

	//parsing GameObjects
	XMLElement* ObjRoot = root->FirstChildElement("GameObjects");
	
	xmlElem = ObjRoot->FirstChildElement("object");
	for (ObjectsCount; xmlElem != NULL; ++ObjectsCount, xmlElem = xmlElem->NextSiblingElement());

	lvlobjects = new GObject*[ObjectsCount];
	xmlElem = ObjRoot->FirstChildElement("object");

	for (int i = 0; i < ObjectsCount; ++i)
	{
		if (!strcmp(xmlElem->Attribute("type"), "Player"))
		{
			lvlobjects[i] = new Player;
		}
		//if (!strcmp(xmlElem->Attribute("type"), "NPC"))
		//{
		//	lvlobjects[i] = new NPC;
		//}
		if (!strcmp(xmlElem->Attribute("type"), "Object"))
		{
			lvlobjects[i] = new GObject;
		}
		if (!lvlobjects[i]->Parse(ObjRoot, i))
		{
			return false;
		}

		xmlElem = xmlElem->NextSiblingElement();
	}
	return true;
}


void Level::Draw()
{
	for (int i = 0; i < TileLayerCount; ++i)
	{ 
		layers[i].Draw();
	}

	for (int i = 0; i < ObjectsCount; ++i)
	{
		lvlobjects[i]->Draw();
	}

}

void Level::Update()
{
	Collision();
	for (int i = 0; i < ObjectsCount; ++i)
	{
		lvlobjects[i]->Update();
	}

}


void Level::Collision()
{
	Hitbox * hitbox;
	for (int i = 0; i < ObjectsCount; ++i)
	{

		if (!lvlobjects[i]->IsColliding()) {continue;}

		hitbox = lvlobjects[i]->GetHitbox();

		if (layers[0].GetGrid(hitbox->y / 32 +1, hitbox->x / 32) || layers[0].GetGrid(hitbox->y / 32 + 1, (hitbox->x + hitbox->w) / 32))
		{
			lvlobjects[i]->setFlagBelow(true);
		}
		else
		{
			lvlobjects[i]->setFlagBelow(false);  
		}

		//if (layers[0].GetGrid(hitbox->y / 32, hitbox->x / 32 + 1))
		//{
		//	lvlobjects[i]->setFlagRight(true);
		//}
		//else
		//{
		//	lvlobjects[i]->setFlagRight(false);
		//}

		//if (layers[0].GetGrid((hitbox->y) / 32, hitbox->x / 32) || layers[0].GetGrid((hitbox->y + hitbox->h) / 32, hitbox->x / 32))
		//{
		//	lvlobjects[i]->setFlagLeft(true);
		//}
		//else
		//{
		//	lvlobjects[i]->setFlagLeft(false);
		//}

	

	}

}