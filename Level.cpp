#include "Vendor/tinyxml2.h"
#include <typeinfo>
#include <iostream>
using namespace tinyxml2;
#include "Level.h"
#include "Game.h"
#include "TileLayer.h"
#include "Background.h"
#include "ElderScroll.h"
#include "MovableObject.h"
#include "FixObject.h"
#include "Boss.h"

Uint16 Level::Levelid = 0;
//bool Level::GameMode = false;
float Level::DifficultyAmpl = 1.f;
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
	IsLoaded = false;
}

bool Level::Load()
{
	std::string path;
	switch (Levelid)
	{
	case 0:
		Game::Log << "Wrong loading";
		return false;
	case 1:
		path = std::string("assets/level1.xml");
		break;
	case 2:
		path = std::string("assets/level2.xml");
		break;
	case 3:
		path = std::string("assets/level3.xml");
		break;
	}

	ElderScroll::InstanceCount = 0;

	XMLDocument lvlDocument;

	if (lvlDocument.LoadFile(path.c_str()))
	{

		Game::Log << "Loading XML file error, " << lvlDocument.ErrorID();
		return false;
	}

	XMLElement* root = lvlDocument.RootElement();   
	

	XMLElement* mapRoot = root->FirstChildElement("map");

	mapRoot->QueryIntAttribute("width", &mapWidth);
	mapRoot->QueryIntAttribute("height", &mapHeigth);

	if (mapWidth <= 0 || mapHeigth <= 0)
		throw std::string("Invalid map dimension parameters \n");

	camera = new Camera(mapWidth, mapHeigth);

	//parsing TileLayers
	XMLElement* LayerRoot = mapRoot->FirstChildElement("Layers");
	
	if(!LayerRoot)
		throw std::string("Bad XML, no Layers root found \n");

	LayerRoot->QueryIntAttribute("physicalLayer", &physicLayerid);


	XMLElement* xmlElem = LayerRoot->FirstChildElement("layer");
	
	if (!xmlElem)
		throw std::string("Bad XML, no layer found \n");

	for (LayerCount; xmlElem != NULL; ++LayerCount, xmlElem = xmlElem->NextSiblingElement());
	
	layers = new Layer*[LayerCount];
	
	xmlElem = LayerRoot->FirstChildElement("layer");
	for (int i = 0; i < LayerCount; ++i)
	{
		const char* type = xmlElem->Attribute("type");
		if (type == nullptr) { throw std::string("Bad stream. Cannot properly read from file. \n");}

		layers[i] = nullptr;
		if (!strcmp(type, "TileLayer"))
		{
			layers[i] = new TileLayer;
		}
		if (!strcmp(type, "Background"))
		{
			layers[i] = new Background;
		}

		if (!layers[i])
			throw std::string("Incorect type specifier for layer \n");

		//if (!layers[i]->Parse(LayerRoot, i))
		//{
		//	return false;
		//}
		try {
			layers[i]->Parse(LayerRoot, i);
		}
		catch(std::string s) { throw s; }
		
		
		xmlElem = xmlElem->NextSiblingElement();
	}

	if(physicLayerid < 0 || physicLayerid > LayerCount || !dynamic_cast<TileLayer*>(layers[physicLayerid]))
		throw std::string("Incorect type specifier for layer \n");

	//Loading object textures
	XMLElement* SoundRoot = root->FirstChildElement("Sounds");

	if (SoundRoot)
	{
		if (xmlElem = SoundRoot->FirstChildElement("levelmusic"))
		{
			BackMusic.Load(xmlElem->Attribute("path"));
			BackMusic.Play();
		}

		xmlElem = SoundRoot->FirstChildElement("sound");
		while (xmlElem != NULL)
		{
			Soundtype mode = Soundtype::Chunk;
			if (const char* type = xmlElem->Attribute("type"))
			{
				if (!strcmp(type, "music"))
				{
					mode = Soundtype::Music;
				}
			}
			if (!SoundManager::GetInstance().Load(xmlElem->Attribute("path"), mode))
				throw std::string("Failed to load sound from path \n");

			xmlElem = xmlElem->NextSiblingElement();
		}
	}


	//Loading object textures
	XMLElement* TexRoot = root->FirstChildElement("Textures");

	//if (!TexRoot)
	//	throw std::string("Bad XML, no Textures root found \n");

	if (TexRoot)
	{
		xmlElem = TexRoot->FirstChildElement("texture");
		while (xmlElem != NULL)
		{
			if (!TextureManager::GetInstance().Load(xmlElem->Attribute("path"), xmlElem->IntAttribute("id")))
				throw std::string("Failed to load texture from path \n");

			xmlElem = xmlElem->NextSiblingElement();
		}
	}

	//parsing GameObjects
	XMLElement* ObjRoot = root->FirstChildElement("GameObjects");
	if (!ObjRoot)
		throw std::string("Bad XML, no GameObjects root found \n");

	for (xmlElem = ObjRoot->FirstChildElement("object"); xmlElem != NULL; xmlElem = xmlElem->NextSiblingElement("object"))
	{
		const char* type = xmlElem->Attribute("type");
		if (type == nullptr) { throw std::string("Bad stream. Cannot properly read from file."); }

		if (!strcmp(type, "Player"))
		{
			XMLElement* subElem = xmlElem->FirstChildElement("playerClass");

			if (subElem == nullptr) { throw std::string("Bad stream. Cannot properly read from file."); }

			while (subElem != NULL)
			{
				const char* Class = subElem->Attribute("type");

				if (Class == nullptr) { throw std::string("Bad stream. Cannot properly read from file."); }

				if (!strcmp(Class, "Swordsman") && (PlayerClass == PlayerClasses::Swordswman))
				{
					Objlist.push_front(new Swordsman);
					if (!Objlist.front()->Parse(ObjRoot, 0, subElem)) { return false; }
					Player::PlayerDead = false;
					break;
				}

				if (!strcmp(Class, "Archer") && (PlayerClass == PlayerClasses::Archer))
				{
					Objlist.push_front(new Archer);
					if (!Objlist.front()->Parse(ObjRoot, 0, subElem)) { return false; }
					Player::PlayerDead = false;
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
		if (!strcmp(type, "DeathBringer"))
		{
			Objlist.push_back(new DeathBringer);
			Boss::IsDefeated = false;
		}
		if (!strcmp(type, "SlimeBoss"))
		{
			Objlist.push_back(new SlimeBoss);
			Boss::IsDefeated = false;
		}
		if (!strcmp(type, "FireDemon"))
		{
			Objlist.push_back(new FireDemon);
			Boss::IsDefeated = false;
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

		if (!Objlist.empty() && !Objlist.back()->Parse(ObjRoot, 0, xmlElem))
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

	if (!Objlist.empty())
	{
		for (GObject* obj : Objlist)
		{
			obj->Draw(camera->GetCameraPos());
		}
	}
}

void Level::Update()
{
	if (!Objlist.empty())
	{
		Collision();

		for (std::list<GObject*>::iterator obj = Objlist.begin(); obj != Objlist.end(); obj++)
		{
			(*obj)->Update();
			if (!(*obj)->shouldExist())
			{
				delete (*obj);
				obj = --Objlist.erase(obj);
			}
		}

		camera->Update(Objlist.front()->GetHitbox());
	}
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
				//hitbox->y = hitbox->y - ((hitbox->y + hitbox->h) - ((int)(hitbox->y + hitbox->h) / tileHeight) * tileHeight);
				hitbox->y = - hitbox->h + ((int)(hitbox->y + hitbox->h) / tileHeight) * tileHeight;
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

		if (hb1->y + hb1->h > hb2->y && hb1->y + hb1->h - hb2->y < hb2->h && ((hb1->x + hb1->w) - hb2->x) > 4 && ((hb2->x + hb2->w) - hb1->x) > 4)
		{
			hb1->y = hb2->y - hb1->h; //PushBack up
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
			Objlist.push_back(new ProjectileObject(archer->getProjectile()));
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

		if (((hb1->y + hb1->h) - hb2->y) > 4 && ((hb2->y + hb2->h) - hb1->y) > 4 && (player->ViewDirection() == Direction::Left && ((hb2->x + hb2->w) > (hb1->x - player->AtackRange())) && ((hb2->x + hb2->w) - (hb1->x - player->AtackRange())) < (player->AtackRange() + 4) || player->ViewDirection() == Direction::Right && ((hb1->x + hb1->w + player->AtackRange()) > (hb2->x)) && ((hb1->x + hb1->w + player->AtackRange()) - (hb2->x)) < player->AtackRange() + 4))
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

	if (dynamic_cast<ElderScroll*>(object))
	{
		ElderScroll* scroll = dynamic_cast<ElderScroll*>(object);
		
		if(!scroll->CanBePicked()) return;

		if (hb1->y + hb1->h <= hb2->y) return;

		if (hb1->y >= hb2->y + hb2->h) return;

		if (hb1->x + hb1->w <= hb2->x) return;

		if (hb1->x >= hb2->x + hb2->w) return;

		scroll->SetExistenceMode(false);
		return;

	}
}

void Level::InteractionBetween(NPC* npc, Player* player)
{
	if (!npc || !player) { SDL_Log("Runtime error, when downcasting"); return; }

	if (npc->IsDead() && npc->GetDrop())
	{
		Objlist.push_back(new ElderScroll(*(npc->GetDrop())));
		npc->IsInteracting(false);
		return;
	}

	Hitbox* hb1 = npc->GetHitbox();
	Hitbox* hb2 = player->GetHitbox();

	if (((hb1->y + hb1->h) - hb2->y) > 4 && ((hb2->y + hb2->h) - hb1->y) > 4 && (npc->ViewDirection() == Direction::Left && ((hb2->x + hb2->w) > (hb1->x - npc->AtackRange())) && ((hb2->x + hb2->w) - (hb1->x - npc->AtackRange())) < (npc->AtackRange() + 4) || npc->ViewDirection() == Direction::Right && ((hb1->x + hb1->w + npc->AtackRange()) > (hb2->x)) && ((hb1->x + hb1->w + npc->AtackRange()) - (hb2->x)) < npc->AtackRange() + 4))
	{
		npc->WantToAtack(true);
		if (npc->IsAtacking())
		{
			npc->IsAtacking(false);
			player->TakeDamage(npc->DoDamage());
		}
	}

	if (dynamic_cast<Boss*>(npc))
	{
		Boss* boss = dynamic_cast<Boss*>(npc);
		if (((hb1->y + hb1->h) - hb2->y) > 4 && ((hb2->y + hb2->h) - hb1->y) > 4 && (boss->ViewDirection() == Direction::Left && ((hb2->x + hb2->w) > (hb1->x - boss->getSecondRange())) && ((hb2->x + hb2->w) - (hb1->x - boss->getSecondRange())) < boss->getSecondRange() || boss->ViewDirection() == Direction::Right && ((hb1->x + hb1->w + boss->getSecondRange()) > (hb2->x)) && ((hb1->x + hb1->w + boss->getSecondRange()) - (hb2->x)) < boss->getSecondRange()))
		{
			if (dynamic_cast<DeathBringer*>(boss))
			{
				DeathBringer* dboss = dynamic_cast<DeathBringer*>(boss);
				dboss->WantToUseAbility(true);
				Hitbox* hb1 = dboss->getAbilityHitbox();

				if (!dboss->IsAbilitySpawnCoordsSet())
				{
					dboss->SetAbilitySpawnCoords({(int)hb2->x + hb2->w/2 - hb1->w/2, (int)hb2->y + hb2->h - hb1->h });
				}
				if (dboss->UsingAbility() && !dboss->IsAtacking())
				{
					if (!(hb1->y + hb1->h <= hb2->y || hb1->y >= hb2->y + hb2->h || hb1->x + hb1->w <= hb2->x || hb1->x >= hb2->x + hb2->w))
					{
						player->TakeDamage(dboss->DoDamage());
					}
				}
			}
			if (dynamic_cast<SlimeBoss*>(boss))
			{
				boss->WantToAtack(true);
				if (boss->IsAtacking())
				{
					boss->IsAtacking(false);
					SlimeBoss* sboss = dynamic_cast<SlimeBoss*>(boss);
					Objlist.push_back(new ProjectileObject(sboss->getProjectile(Positioning::Top)));
					Objlist.push_back(new ProjectileObject(sboss->getProjectile(Positioning::Middle)));
					Objlist.push_back(new ProjectileObject(sboss->getProjectile(Positioning::Bottom)));
				}
			}
			if (dynamic_cast<FireDemon*>(boss))
			{
				FireDemon* fboss = dynamic_cast<FireDemon*>(boss);
				fboss->WantToUseAbility(true);

				if (fboss->UsingAbility() && !fboss->IsAtacking())
				{
					Objlist.push_back(new ProjectileObject(fboss->getProjectile()));
				}
			}

		}
	}
}

void Level::InteractionBetween(ProjectileObject* projectile, GObject* object)
{
	if (!projectile) { SDL_Log("Runtime error, when downcasting"); return; }

	if (!object->IsCollidingWithObj()) { return; }

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