#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <stdbool.h>
#include "map.h"
#include "../game/constants.h"

#define MAP_NODE_COUNT (MAP_WIDTH * MAP_HEIGHT)

/*
Path represents a sequence of tile coordinates from the start to goal.
The caller own the memory.
No dynamic allocation occurs inside pathfinding.
*/
typedef struct
{
	// Tile coordinates stored as [tx, ty]
	int tiles[MAX_PATH_LENGTH][2];

	int length;

	// Debug visualization data
	bool debug_open[MAP_NODE_COUNT];
	bool debug_closed[MAP_NODE_COUNT];
	bool debug_in_path[MAP_NODE_COUNT];
} Path;

bool Pathfinding_FindPath(
	const Map *map,
	int start_tx,
	int start_ty,
	int goal_tx,
	int goal_ty,
	Path *out_path
);

#endif