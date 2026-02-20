#ifndef MAP_H
#define MAP_H 

#include <stdbool.h>
#include "../game/constants.h"

typedef struct {
	int walkable;  // 1 - walkable, 0 - blocked
	int occupied;  // 1 - unit present, 0 - free
} Tile;

typedef struct {
	Tile tiles[MAP_HEIGHT][MAP_WIDTH];
} Map;

void Map_Init(Map *map);

// Returns true if tile coordinates are inside map bounds
bool Map_IsInside(const Map *map, int tx, int ty);

// Returns true if tile is walkable (terrain-based)
bool Map_IsWalkable(const Map *map, int tx, int ty);

bool Map_IsOccupied(const Map *map, int tx, int ty);
void Map_SetOccupied(Map *map, int tx, int ty, bool value);

#endif
