#include "Vendor/tinyxml2.h"
#include <typeinfo>
#include <iostream>
using namespace tinyxml2;
#include "Level.h"
#include "TileLayer.h"
#include "Background.h"
#include "ElderScroll.h"
#include "MovableObject.h"
#include "FixObject.h"

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


	for (xmlElem = ObjRoot->FirstChildElement("object"); xmlElem != NULL; xmlElem = xmlElem->NextSiblingElement("object"))
	{
		const char* type = xmlElem->Attribute("type");
		if (type == nullptr) { SDL_Log("Bad stream. Cannot properly read from file."); return false; }

		if (!strcmp(type, "Player"))
		{
			XMLElement* subElem = xmlElem->FirstChildElement("playerClass");

			if (subElem == nullptr) { SDL_Log("Bad stream. Cannot properly read from file."); return false; }

			while (subElem != NULL)
			{
				const char* Class = subElem->Attribute("type");

				if (!strcmp(Class, "Swordsman") && (PlayerClass == PlayerClasses::Swordswman))
				{
					Objlist.push_front(new Swordsman);
					if (!Objlist.front()->Parse(ObjRoot, 0, subElem)) { return false; }
					break;
				}

				if (!strcmp(Class, "Archer") && (PlayerClass == PlayerClasses::Archer))
				{
					Objlist.push_front(new Archer);
					if (!Objlist.front()->Parse(ObjRoot, 0, subElem)) { return false; }
					break;
				}
				subElem = subElem->NextSiblingElement("playerClass");
			}
			continue;

		}

		//if (!strcmp(type, "Swordsman"))		//"Upcasting"
		//{
		//	Objlist.push_front(new Swordsman);
		//}
		//if (!strcmp(type, "Archer"))
		//{
		//	Objlist.push_front(new Archer);
		//}

		if (!strcmp(type, "ShortRangeNPC"))
		{
			Objlist.push_back(new ShortRangeNPC);
		}
		if (!strcmp(type, "LongRangeNPC"))
		{
			Objlist.push_back(new LongRangeNPC);
		}
		if (!strcmp(type, "ElderScroll"))
		{
			Objlist.push_back(new ElderScroll);
		}
		if (!strcmp(type, "MovableObject"))
		{
			Objlist.push_back(new MovableObject);
		}
		if (!strcmp(type, "FixedObject"))
		{
			Objlist.push_back(new FixedObject);
		}

		//if (!strcmp(type, "Object"))
		//{
		//	Objlist.push_back(new GObject);
		//}
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

	for (std::list<GObject*>::iterator obj = Objlist.begin();  obj != Objlist.end(); obj++)
	{
		(*obj)->Update();
		if (!(*obj)->shouldExist())
		{
			obj = --Objlist.erase(obj);
		}
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
		if (!object->IsInteracting()) { continue; }

		for (GObject* obiect : Objlist)
		{
			if (!obiect->IsInteracting() || (object == obiect)) { continue; }

			InteractionBetween(object, obiect);
			
		}
	}
	#pragma endregion

}

//void Level::CheckColision(GObject*, GObject*)
//{
//
//}

void Level::InteractionBetween(GObject* first, GObject* second)
{
	if (dynamic_cast <ProjectileObject*>(first))
	{
		InteractionBetween(dynamic_cast <ProjectileObject*>(first), second);
		return;
	}

	if(first->IsCollidingWithObj() && second->IsCollidingWithObj())
	{
		Hitbox* hb1 = first->GetHitbox();
		Hitbox* hb2 = second->GetHitbox();

		if (hb2->x + hb2->w > hb1->x && hb2->x + hb2->w - hb1->x < 4 && ((hb1->y + hb1->h) - hb2->y) > 4 && ((hb2->y + hb2->h) - hb1->y) > 4)
		{
			first->setFlagLeft(true);
			//second->setFlagRight(true);
		}

		if (hb1->x + hb1->w > hb2->x && hb1->x + hb1->w - hb2->x < 4 && ((hb1->y + hb1->h) - hb2->y) > 4 && ((hb2->y + hb2->h) - hb1->y) > 4)
		{
			first->setFlagRight(true);
			//second->setFlagLeft(true);
		}

		if (hb2->y + hb2->h > hb1->y && hb2->y + hb2->h - hb1->y < 4 && ((hb1->x + hb1->w) - hb2->x) > 4 && ((hb2->x + hb2->w) - hb1->x) > 4)
		{
			first->setFlagAbove(true);
			//second->setFlagBelow(true);
		}

		if (hb1->y + hb1->h > hb2->y && hb1->y + hb1->h - hb2->y < 4 && ((hb1->x + hb1->w) - hb2->x) > 4 && ((hb2->x + hb2->w) - hb1->x) > 4)
		{
			first->setFlagBelow(true);
			//second->setFlagAbove(true);
		}

	}


	if (dynamic_cast <Player*>(first))
	{
		InteractionBetween(dynamic_cast <Player*>(first), second);
		return;
	}

	if (dynamic_cast <NPC*>(first) && dynamic_cast <Player*>(second))
	{
		InteractionBetween(dynamic_cast <NPC*>(first), dynamic_cast <Player*>(second));
		return;
	}
}
void Level::InteractionBetween(Player* player, GObject* object)
{
	if (!player) { SDL_Log("Runtime error, when downcasting"); return; }

	Hitbox* hb1 = player->GetHitbox();
	Hitbox* hb2 = object->GetHitbox();

	if (PlayerClass == PlayerClasses::Archer && player->IsAtacking())
	{
			player->IsAtacking(false);
			Archer* archer = dynamic_cast <Archer*>(player);
			GObject* newObj = new ProjectileObject(archer->getProjectile());
			Objlist.push_back(newObj);
	}

	if (dynamic_cast<NPC*>(object))
	{
		NPC* npc = dynamic_cast<NPC*>(object);

		if (((hb1->y + hb1->h) - hb2->y) > 4 && ((hb2->y + hb2->h) - hb1->y) > 4 && (player->ViewDirection() == Direction::Left && ((hb2->x + hb2->w) > (hb1->x - player->AtackRange())) && ((hb2->x + hb2->w) - (hb1->x )) < player->AtackRange() || player->ViewDirection() == Direction::Right && ((hb1->x + hb1->w + player->AtackRange()) > (hb2->x)) && ((hb1->x + hb1->w ) - (hb2->x)) < player->AtackRange()))
		{
			if (PlayerClass == PlayerClasses::Swordswman && player->IsAtacking())
			{
				player->IsAtacking(false);
				npc->TakeDamage(player->DoDamage());
			}
		}
		return;
	}

	if (dynamic_cast<MovableObject*>(object))
	{
		MovableObject* mobject = dynamic_cast<MovableObject*>(object);

		if (((hb1->y + hb1->h) - hb2->y) > 4 && ((hb2->y + hb2->h) - hb1->y) > 4 && (player->ViewDirection() == Direction::Left && ((hb2->x + hb2->w) > (hb1->x - player->AtackRange())) && ((hb2->x + hb2->w) - (hb1->x)) < player->AtackRange() || player->ViewDirection() == Direction::Right && ((hb1->x + hb1->w + player->AtackRange()) > (hb2->x)) && ((hb1->x + hb1->w ) - (hb2->x)) < player->AtackRange()))
		{
			if (player->IsAtacking())
			{
				player->IsAtacking(false);
				mobject->Move(player->ViewDirection());
			}
		}

		if (((hb1->y + hb1->h) - hb2->y) > 4 && ((hb2->y + hb2->h) - hb1->y) > 4 && (player->ViewDirection() == Direction::Left && ((hb2->x + hb2->w) > (hb1->x - player->getHangRange())) && ((hb2->x + hb2->w) - (hb1->x - player->getHangRange())) < player->getHangRange() || player->ViewDirection() == Direction::Right && ((hb1->x + hb1->w + player->getHangRange()) > (hb2->x)) && ((hb1->x + hb1->w + player->getHangRange()) - (hb2->x)) < player->getHangRange()))
		{
			if (player->IsHanging())
			{
				mobject->Move(~(player->ViewDirection()));
			}
		}
		return;
	}
}

void Level::InteractionBetween(NPC* npc, Player* player)
{
	if (!npc || !player) { SDL_Log("Runtime error, when downcasting"); return; }
	Hitbox* hb1 = npc->GetHitbox();
	Hitbox* hb2 = player->GetHitbox();

	if (((hb1->y + hb1->h) - hb2->y) > 4 && ((hb2->y + hb2->h) - hb1->y) > 4 && (npc->ViewDirection() == Direction::Left && ((hb2->x + hb2->w) > (hb1->x - npc->AtackRange())) && ((hb2->x + hb2->w) - (hb1->x - npc->AtackRange())) < npc->AtackRange() || npc->ViewDirection() == Direction::Right && ((hb1->x + hb1->w + npc->AtackRange()) > (hb2->x)) && ((hb1->x + hb1->w + npc->AtackRange()) - (hb2->x)) < npc->AtackRange()) )
	{
		npc->WantToAtack(true);
	}
	if (npc->IsAtacking())
	{
		npc->IsAtacking(false);
		player->TakeDamage(npc->DoDamage());
	}
}

void Level::InteractionBetween(ProjectileObject* projectile, GObject* object)
{
	if (!projectile) { SDL_Log("Runtime error, when downcasting"); return; }

	Hitbox* hb1 = projectile->GetHitbox();
	Hitbox* hb2 = object->GetHitbox();

	if((projectile->FlyDirection() == Direction::Left && (hb2->x + hb2->w/2) > hb1->x && (hb2->x + hb2->w / 2) - hb1->x < 4 || projectile->FlyDirection() == Direction::Right && (hb1->x + hb1->w/2) > hb2->x && (hb1->x + hb1->w / 2) - hb2->x < 4) && (hb1->y + hb1->h) > hb2->y && (hb2->y + hb2->h) > hb1->y)
	{
		projectile->SetExistenceMode(false);
		if (dynamic_cast<Entity*>(object))
		{
			Entity* entity = dynamic_cast<Entity*>(object);
			entity->TakeDamage(projectile->DoDamage());
		}
	}
}