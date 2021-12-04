#pragma once
#include "TileLayer.h"
#include "Player.h"

class Level
{
	int mapWidth;
	int mapHeigth;


	TileLayer* layers; //First layer is the Physic layer
	int TileLayerCount;

	GObject** lvlobjects;
	int ObjectsCount;
public:
	Level() : mapWidth(0), mapHeigth(0), layers(nullptr), TileLayerCount(0), lvlobjects(nullptr), ObjectsCount(0)
	{}
	~Level() { delete[]layers; delete[]lvlobjects; } //VErifica


	bool LvLparser(const char* path);
	void Draw();
	void Update();

	void Collision();
};

