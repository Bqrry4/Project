#include "Vendor/tinyxml2.h"
#include "Level.h"
#include <typeinfo>


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


	//parsing TileLayers
	XMLElement* xmlElem = mapRoot->FirstChildElement("layer");
	for (TileLayerCount; xmlElem != NULL; ++TileLayerCount, xmlElem = xmlElem->NextSiblingElement());
	
	layers = new TileLayer[TileLayerCount];
	
	for (int i = 0; i < TileLayerCount; ++i)
	{
		if (!layers[i].Parse(mapRoot, i))
		{
			return false;
		}
	}

	//Loading object textures
	XMLElement* TexRoot = root->FirstChildElement("Textures");

	xmlElem = TexRoot->FirstChildElement("texture");
	while (xmlElem != NULL)
	{
		if (!TextureManager::GetInstance()->Load(xmlElem->Attribute("path"), xmlElem->IntAttribute("id"))) { return false; }

		xmlElem = xmlElem->NextSiblingElement();
	}




	//parsing GameObjects
	XMLElement* ObjRoot = root->FirstChildElement("GameObjects");
	
	xmlElem = ObjRoot->FirstChildElement("object");
	for (ObjectsCount; xmlElem != NULL; ++ObjectsCount, xmlElem = xmlElem->NextSiblingElement("object"));


	lvlobjects = new GObject*[ObjectsCount];
	xmlElem = ObjRoot->FirstChildElement("object");

	for (int i = 0; i < ObjectsCount; ++i)
	{
		if (!strcmp(xmlElem->Attribute("type"), "Swordsman"))		//"Upcasting"
		{
			lvlobjects[i] = new Swordsman;
		}
		if (!strcmp(xmlElem->Attribute("type"), "Archer"))		//"Upcasting"
		{
			lvlobjects[i] = new Archer;
		}

		if (!strcmp(xmlElem->Attribute("type"), "ShortRangeNPC"))
		{
			lvlobjects[i] = new ShortRangeNPC;
		}
		if (!strcmp(xmlElem->Attribute("type"), "LongRangeNPC"))
		{
			lvlobjects[i] = new LongRangeNPC;
		}
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

	#pragma region Colision with Map
	{//Stackframe
		Hitbox* hitbox;
		int tileWidth = layers[0].GetGridW();
		int tileHeight = layers[0].GetGridH();

		for (int i = 0; i < ObjectsCount; ++i)  //Check for colision around objects with map
		{

			if (!lvlobjects[i]->IsColliding()) { continue; }


			hitbox = lvlobjects[i]->GetHitbox();


			if (layers[0].GetGrid(((hitbox->y + 4) / tileHeight) + 1, hitbox->x / tileWidth) || layers[0].GetGrid((hitbox->y + 4) / tileHeight + 1, (hitbox->x + hitbox->w) / tileWidth))
			{
				lvlobjects[i]->setFlagBelow(true);
			}
			else
			{
				lvlobjects[i]->setFlagBelow(false);
			}

			if (layers[0].GetGrid((hitbox->y + 4) / tileHeight, (hitbox->x + hitbox->w + 4) / tileWidth) || layers[0].GetGrid((hitbox->y + hitbox->h - 4) / tileHeight, (hitbox->x + hitbox->w + 4) / tileWidth))
			{
				lvlobjects[i]->setFlagRight(true);
			}
			else
			{
				lvlobjects[i]->setFlagRight(false);
			}

			if (layers[0].GetGrid((hitbox->y + 4) / tileHeight, (hitbox->x - 4) / tileWidth) || layers[0].GetGrid((hitbox->y + hitbox->h - 4) / tileHeight, (hitbox->x - 4) / tileWidth))
			{
				lvlobjects[i]->setFlagLeft(true);
			}
			else
			{
				lvlobjects[i]->setFlagLeft(false);
			}

			if (layers[0].GetGrid((hitbox->y / tileHeight), hitbox->x / tileWidth) || layers[0].GetGrid(hitbox->y / tileHeight, (hitbox->x + hitbox->w) / tileWidth))
			{
				lvlobjects[i]->setFlagAbove(true);
			}
			else
			{
				lvlobjects[i]->setFlagAbove(false);
			}

		}
	}
	#pragma endregion


	#pragma region Colision Between Objects
	for (int i = 0; i < ObjectsCount; ++i)  //Check for colision between objects
	{
		if (!lvlobjects[i]->IsCollidingWithObj()) { continue; }

		for (int j = 0; j < ObjectsCount; ++j)
		{
			if (!lvlobjects[j]->IsCollidingWithObj() || (i == j)) { continue; }

			InteractionBetween(lvlobjects[i], lvlobjects[j]);
			
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
	SDL_Log("Aici");
	if (player->ViewDirection() == Looking::Left) {
		if (((hb2->x + hb2->w) > (hb1->x - player->AtackRange())) && ((hb2->x + hb2->w) - (hb1->x - player->AtackRange())) < player->AtackRange() && ((hb1->y + hb1->h) - hb2->y) > 4)
		{
			if((hb2->x + hb2->w) - (hb1->x) < 1)
			{
				player->setFlagLeft(true);
			}
			if (player->IsAtacking())
			{
				npc->TakeDamage(player->DoDamage());
			}

		}

	}
	if (player->ViewDirection() == Looking::Right) {
		if (((hb1->x + hb1->w + player->AtackRange()) > (hb2->x)) && ((hb1->x + hb1->w + player->AtackRange()) - (hb2->x)) < player->AtackRange() && ((hb1->y + hb1->h) - hb2->y) > 4)
		{
			if ((hb1->x + hb1->w) - (hb2->x) < 1)
			{
				player->setFlagRight(true);
			}
			if (player->IsAtacking())
			{
				npc->TakeDamage(player->DoDamage());
			}

		}
	}
}

void Level::InteractionBetween(NPC* npc, Player* player)
{
	if (!npc || !player) { SDL_Log("Runtime error, when downcasting"); return; }
	Hitbox* hb1 = npc->GetHitbox();
	Hitbox* hb2 = player->GetHitbox();

	if (npc->ViewDirection() == Looking::Left) {
		if (((hb2->x + hb2->w) > (hb1->x - npc->AtackRange())) && ((hb2->x + hb2->w) - (hb1->x - npc->AtackRange())) < npc->AtackRange() && ((hb1->y + hb1->h) - hb2->y) > 4)
		{
			npc->WantToAtack(true);

			if(npc->IsAtacking())
			{
				player->TakeDamage(npc->DoDamage());
			}

		}

	}
	if (npc->ViewDirection() == Looking::Right) {
		if (((hb1->x + hb1->w + npc->AtackRange()) > (hb2->x)) && ((hb1->x + hb1->w + npc->AtackRange()) - (hb2->x)) < npc->AtackRange() && ((hb1->y + hb1->h) - hb2->y) > 4)
		{
			npc->WantToAtack(true);
			if (npc->IsAtacking())
			{
				player->TakeDamage(npc->DoDamage());
			}

		}
	}
}