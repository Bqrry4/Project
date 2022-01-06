#pragma once
#include "Camera.h"
#include "Layer.h"
#include "Swordsman.h"
#include "Archer.h"
#include "ShortRangeNPC.h"
#include "LongRangeNPC.h"
#include <list>

enum class PlayerClasses {

	Swordswman,
	Archer,
	Mage
};


class Level
{
	int mapWidth;
	int mapHeigth;
	Camera* camera;

	Layer** layers; //First layer is the Physic layer
	int LayerCount;
	int physicLayerid;


	//GObject** lvlobjects; //Vector of loaded objects
	//int ObjectsCount;

	std::list<GObject*> Objlist; //List of level's objects

public:
	static Uint16 Levelid;
	static bool GameMode; // 0 = Single mode, 1 = Duo mode
	static PlayerClasses PlayerClass;
	static bool IsLoaded;

	Level() : mapWidth(0), mapHeigth(0), camera(nullptr), layers(nullptr), LayerCount(0), physicLayerid(0)
	{}
	~Level();

	bool Load();
	//bool Unload();

	void Draw();
	void Update();

	void Collision();
	//void CheckColision(GObject*, GObject*);
	
	//void DownCasting(GObject*);
	void InteractionBetween(GObject*, GObject*);
	void InteractionBetween(Player*, GObject*);
	void InteractionBetween(NPC*, Player*);

	void InteractionBetween(ProjectileObject*, GObject*);
	//void InteractionBetween(ProjectileObject*, NPC*);

	void PathFinding(SDL_Point start, SDL_Point end) //Using A* - algorythm
	{}
};

