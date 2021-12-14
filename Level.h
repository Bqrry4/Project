#pragma once
#include "Camera.h"
#include "TileLayer.h"
#include "Swordsman.h"
#include "Archer.h"
#include "ShortRangeNPC.h"
#include "LongRangeNPC.h"


class Level
{
	int mapWidth;
	int mapHeigth;
	Camera* camera;

	TileLayer* layers; //First layer is the Physic layer
	int TileLayerCount;

	GObject** lvlobjects;
	int ObjectsCount;
public:

	static Uint16 Levelid;
	static bool GameMode; // 0 = Single mode, 1 = Duo mode
	static Uint16 PlayerClass;
	static bool IsLoaded;

	Level() : mapWidth(0), mapHeigth(0), camera(nullptr), layers(nullptr), TileLayerCount(0), lvlobjects(nullptr), ObjectsCount(0)
	{}
	~Level() { delete[]layers; delete[]lvlobjects; delete camera; }


	bool LvLparser(const char* path);
	void Draw();
	void Update();

	void Collision();
	
	//void DownCasting(GObject*);
	void InteractionBetween(GObject*, GObject*);
	void InteractionBetween(Player*, NPC*);
	void InteractionBetween(NPC*, Player*);
};

