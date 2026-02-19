#ifndef MAP_H
#define MAP_H 

#include "raylib.h"

#define MAP_WIDTH 20
#define MAP_HEIGHT 15
#define TILE_SIZE 32

typedef struct {
	int walkable;
} Tile;

typedef struct {
	Tile tiles[MAP_HEIGHT][MAP_WIDTH];
} Map;

void Map_Init(Map* map);
int Map_IsInside(Map* map, int tx, int ty);

Vector2 Map_TileToWorld(int tx, int ty);
Vector2 Map_WorldToTile(float wx, float wy);

#endif
