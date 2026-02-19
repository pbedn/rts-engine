#ifndef PATHFINDING_H
#define PATHFINDING_H

#include <stdbool.h>
#include "map.h"
#include "unit.h"


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