#include "Vendor/tinyxml2.h"
#include <typeinfo>
#include <iostream>
using namespace tinyxml2;
#include "Level.h"
#include "TileLayer.h"
#include "Background.h"

Uint16 Level::Levelid = 0;
bool Level::GameMode = false;
PlayerClasses Level::PlayerClass = PlayerClasses::Swordswman;
bool Level::IsLoaded = false;


Level::~Level()
{ 
	for (int i = 0; i < LayerCount; ++i) { delete layers[i]; }
	delete[]layers;
	//for (int i = 0; i < ObjectsCount; ++i) { delete lvlobjects[i]; }
	//delete[]lvlobjects;
	for (GObject* obj : Objlist) { delete obj; }
	Objlist.clear();
	delete camera;
}

bool Level::Load()
{
	std::string path;
	switch (Levelid)
	{
	case 0:
		SDL_Log("Wrong loading");
		return false;
	case 1:
		path = std::string("assets/map1.tmx");
		break;
	case 2:
		path = std::string("assets/level2.xml");
		break;
	case 3:
		path = std::string("assets/level3.xml");
		break;
	}


	XMLDocument lvlDocument;

	if (lvlDocument.LoadFile(path.c_str()))  ///ERROR HANDLING DONT FORGET!
	{

		SDL_Log("Loading XML file error, %d", lvlDocument.ErrorID());
		return false;
	}

	XMLElement* root = lvlDocument.RootElement();   
	

	XMLElement* mapRoot = root->FirstChildElement("map");

	mapRoot->QueryIntAttribute("width", &mapHeigth);
	mapRoot->QueryIntAttribute("height", &mapWidth);
	camera = new Camera(mapHeigth, mapWidth);

	//parsing TileLayers
	XMLElement* LayerRoot = mapRoot->FirstChildElement("Layers");
	LayerRoot->QueryIntAttribute("physicalLayer", &physicLayerid);

	XMLElement* xmlElem = LayerRoot->FirstChildElement("layer");
	for (LayerCount; xmlElem != NULL; ++LayerCount, xmlElem = xmlElem->NextSiblingElement());
	
	layers = new Layer*[LayerCount];
	
	xmlElem = LayerRoot->FirstChildElement("layer");
	for (int i = 0; i < LayerCount; ++i)
	{
		const char* type = xmlElem->Attribute("type");
		if (type == nullptr) { SDL_Log("Bad stream. Cannot properly read from file."); return false; }

		if (!strcmp(type, "TileLayer"))
		{
			layers[i] = new TileLayer;
		}
		if (!strcmp(type, "Background"))
		{
			layers[i] = new Background;
		}
		if (!layers[i]->Parse(LayerRoot, i))
		{
			return false;
		}
		xmlElem = xmlElem->NextSiblingElement();
	}

	//Loading object textures
	XMLElement* TexRoot = root->FirstChildElement("Textures");

	xmlElem = TexRoot->FirstChildElement("texture");
	while (xmlElem != NULL)
	{
		if (!TextureManager::GetInstance().Load(xmlElem->Attribute("path"), xmlElem->IntAttribute("id"))) { return false; }

		xmlElem = xmlElem->NextSiblingElement();
	}

	//parsing GameObjects
	XMLElement* ObjRoot = root->FirstChildElement("GameObjects");
	
	//xmlElem = ObjRoot->FirstChildElement("object");
	//for (ObjectsCount; xmlElem != NULL; ++ObjectsCount, xmlElem = xmlElem->NextSiblingElement("object"));


	//lvlobjects = new GObject*[ObjectsCount];


	for (xmlElem = ObjRoot->FirstChildElement("object"); xmlElem != NULL; xmlElem = xmlElem->NextSiblingElement("object"))
	{
		const char* type = xmlElem->Attribute("type");
		if (type == nullptr) { SDL_Log("Bad stream. Cannot properly read from file."); return false; }

		//if (!strcmp(type, "Player"))
		//{
		//	const char* Class = xmlElem->Attribute("class");
		//	if (!strcmp(type, "Swordsman") || Class == nullptr)
		//	{
		//		lvlobjects[i] = new Swordsman;
		//	}
		//	if (!strcmp(type, "Archer"))
		//	{
		//		lvlobjects[i] = new Archer;
		//	}
		//}

		if (!strcmp(type, "Swordsman"))		//"Upcasting"
		{
			Objlist.push_front(new Swordsman);
		}
		if (!strcmp(type, "Archer"))
		{
			Objlist.push_front(new Archer);
		}
		if (!strcmp(type, "ShortRangeNPC"))
		{
			Objlist.push_back(new ShortRangeNPC);
		}
		if (!strcmp(type, "LongRangeNPC"))
		{
			Objlist.push_back(new LongRangeNPC);
		}
		if (!strcmp(type, "Object"))
		{
			Objlist.push_back(new GObject);
		}
		if (!Objlist.back()->Parse(ObjRoot, 0, xmlElem))
		{
			return false;
		}

	}
	IsLoaded = true;
	return true;
}

//bool Level::Unload()
//{
//	//Level::~Level();
//}

void Level::Draw()
{
	for (int i = 0; i < LayerCount; ++i)
	{ 
		layers[i]->Draw(camera->GetCameraPos());
	}

	for (GObject* obj : Objlist)
	{
		obj->Draw(camera->GetCameraPos());
	}

}

void Level::Update()
{
	Collision();

	for (GObject* obj : Objlist)
	{
		obj->Update();
	}

	camera->Update(Objlist.front()->GetHitbox());
}


void Level::Collision()
{

	#pragma region Colision with Map
	{//Stackframe

		Hitbox* hitbox;
		TileLayer* physicLayer = dynamic_cast<TileLayer*>(layers[physicLayerid]);
		if(physicLayer == nullptr) { SDL_Log("Runtime error, when downcasting"); return; } //throw exxeption

		int tileWidth = physicLayer->GetGridW();
		int tileHeight = physicLayer->GetGridH();

		for (GObject* obj : Objlist)  //Check for colision around objects with map
		{

			if (!obj->IsColliding()) { continue; }


			hitbox = obj->GetHitbox();


			if (physicLayer->GetGrid(((hitbox->y + hitbox->h ) / tileHeight), hitbox->x / tileWidth) || physicLayer->GetGrid((hitbox->y + hitbox->h) / tileHeight, (hitbox->x + hitbox->w) / tileWidth))
			{
				hitbox->y = hitbox->y - ((hitbox->y + hitbox->h) - ((int)(hitbox->y + hitbox->h) / tileHeight) * tileHeight);
				obj->setFlagBelow(true);
			}
			else
			{
				obj->setFlagBelow(false);
			}

			if (physicLayer->GetGrid((hitbox->y + 4) / tileHeight, (hitbox->x + hitbox->w + 4) / tileWidth) || physicLayer->GetGrid((hitbox->y + hitbox->h - 4) / tileHeight, (hitbox->x + hitbox->w + 4) / tileWidth))
			{
				obj->setFlagRight(true);
			}
			else
			{
				obj->setFlagRight(false);
			}

			if (physicLayer->GetGrid((hitbox->y + 4) / tileHeight, (hitbox->x - 4) / tileWidth) || physicLayer->GetGrid((hitbox->y + hitbox->h - 4) / tileHeight, (hitbox->x - 4) / tileWidth))
			{
				obj->setFlagLeft(true);
			}
			else
			{
				obj->setFlagLeft(false);
			}

			if (physicLayer->GetGrid((hitbox->y / tileHeight), hitbox->x / tileWidth) || physicLayer->GetGrid(hitbox->y / tileHeight, (hitbox->x + hitbox->w) / tileWidth))
			{
				obj->setFlagAbove(true);
			}
			else
			{
				obj->setFlagAbove(false);
			}

		}
	}
	#pragma endregion


	#pragma region Colision Between Objects
	for (GObject* object : Objlist)  //Check for colision between objects
	{
		if (!object->IsCollidingWithObj()) { continue; }

		for (GObject* obiect : Objlist)
		{
			if (!obiect->IsCollidingWithObj() || (object == obiect)) { continue; }

			InteractionBetween(object, obiect);
			
		}
	}
	#pragma endregion

}


void Level::InteractionBetween(GObject* first, GObject* second)
{
	//DownCasting
	if (first->GetObjectClassId() == 2 && second->GetObjectClassId() == 3 )
	{
		InteractionBetween(dynamic_cast <Player*>(first), dynamic_cast <NPC*>(second));
		return;
	}

	if (first->GetObjectClassId() == 3 && second->GetObjectClassId() == 2)
	{
		InteractionBetween(dynamic_cast <NPC*>(first), dynamic_cast <Player*>(second));
		return;
	}

}

void Level::InteractionBetween(Player* player, NPC* npc)
{
	if (!player || !npc) { SDL_Log("Runtime error, when downcasting"); return; }

	Hitbox* hb1 = player->GetHitbox();
	Hitbox* hb2 = npc->GetHitbox();

	if (player->ViewDirection() == Looking::Left)
	{
		if (((hb2->x + hb2->w) > (hb1->x - player->AtackRange()))  && ((hb2->x + hb2->w) - (hb1->x - player->AtackRange())) < player->AtackRange() && ((hb1->y + hb1->h) - hb2->y) > 4 && ((hb2->y + hb2->h) - hb1->y) > 4)
		{
			if((hb2->x + hb2->w) - (hb1->x) < 1)
			{
				player->setFlagLeft(true);
			}
			if (PlayerClass == PlayerClasses::Swordswman)
			{
				if (player->IsAtacking())
				{
					npc->TakeDamage(player->DoDamage());
				}
			}
		}
	}
	if (player->ViewDirection() == Looking::Right)
	{
		if (((hb1->x + hb1->w + player->AtackRange()) > (hb2->x)) && ((hb1->x + hb1->w + player->AtackRange()) - (hb2->x)) < player->AtackRange() && ((hb1->y + hb1->h) - hb2->y) > 4 && ((hb2->y + hb2->h) - hb1->y) > 4)
		{
			if ((hb1->x + hb1->w) - (hb2->x) < 1)
			{
				player->setFlagRight(true);
			}
			if (PlayerClass == PlayerClasses::Swordswman)
			{
				if (player->IsAtacking())
				{
					npc->TakeDamage(player->DoDamage());
				}
			}

		}

	}

	if (PlayerClass == PlayerClasses::Archer)
	{
		if (player->IsAtacking())
		{
			Archer* archer = dynamic_cast <Archer*>(player);
			GObject* newObj = new ProjectileObject(archer->getProjectile());
			Objlist.push_back(newObj);
		}
	}

}

void Level::InteractionBetween(NPC* npc, Player* player)
{
	if (!npc || !player) { SDL_Log("Runtime error, when downcasting"); return; }
	Hitbox* hb1 = npc->GetHitbox();
	Hitbox* hb2 = player->GetHitbox();

	if (npc->ViewDirection() == Looking::Left)
	{
		if (((hb2->x + hb2->w) > (hb1->x - npc->AtackRange())) && ((hb2->x + hb2->w) - (hb1->x - npc->AtackRange())) < npc->AtackRange() && ((hb1->y + hb1->h) - hb2->y) > 4 && ((hb2->y + hb2->h) - hb1->y) > 4)
		{
			npc->WantToAtack(true);

			if(npc->IsAtacking())
			{
				player->TakeDamage(npc->DoDamage());
			}

		}

	}
	if (npc->ViewDirection() == Looking::Right)
	{
		if (((hb1->x + hb1->w + npc->AtackRange()) > (hb2->x)) && ((hb1->x + hb1->w + npc->AtackRange()) - (hb2->x)) < npc->AtackRange() && ((hb1->y + hb1->h) - hb2->y) > 4 && ((hb2->y + hb2->h) - hb1->y) > 4)
		{
			npc->WantToAtack(true);
			if (npc->IsAtacking())
			{
				player->TakeDamage(npc->DoDamage());
			}

		}
	}
}