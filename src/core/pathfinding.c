/*
Pathfinding module.

Implements A* pathfinding on the grid-based Map.

This module:
- Reads Map data
- Does not modify Map
- Does not allocate memory dynamically
- Is fully deterministic
*/

#include <stdbool.h>
#include <stdlib.h>
#include "pathfinding.h"
#include "map.h"

/*
Internal node used by A*.

Each tile in the map corresponds to exactly one PathNode.
Nodes are reused per pathfinding call.
*/
typedef struct
{
	int tx;
	int ty;

	int g_cost;	 // cost from start node
	int h_cost;  // heuristic to goal
	int f_cost;  // g + g

	int parent_index;

	bool opened;
	bool closed;
} PathNode;

/*
Converts tile coordinates to linear index.
Used internally for node array access.
*/
static int Path_Index(int tx, int ty)
{
	return ty * MAP_WIDTH + tx;
};

/*
Returns Manhattan distance between two tiles.
Admissible heuristic for 4-directional grid.
*/
static int Path_Heuristic(int ax, int ay, int bx, int by)
{
	return abs(ax - bx) + abs(ay - by);
};

bool Pathfinding_FindPath(
	const Map *map,
	int start_tx,
	int start_ty,
	int goal_tx,
	int goal_ty,
	Path *out_path
)
{
	// Reset output path
	out_path->length = 0;

	// Basic validation
	if (!Map_IsInside(map, start_tx, start_ty))
		return false;

	if (!Map_IsInside(map, goal_tx, goal_ty))
		return false;

	if (!Map_IsWalkable(map, goal_tx, goal_ty))
		return false;

	// special case: already at goal
	if (start_tx == goal_tx && start_ty == goal_ty)
		return true;

	// create node grid
	PathNode nodes[MAP_WIDTH * MAP_HEIGHT];

	// Initialize all nodes
	for (int ty= 0; ty< MAP_HEIGHT; ty++)
	{
		for (int tx = 0; tx < MAP_WIDTH; tx++)
		{
			int index = Path_Index(tx, ty);

			nodes[index].tx = tx;
			nodes[index].ty = ty;

			nodes[index].g_cost = 0;
			nodes[index].h_cost = 0;
			nodes[index].f_cost = 0;

			nodes[index].parent_index = 0;

			nodes[index].opened = false;
			nodes[index].closed = false;
		}
	}

	// Setup start node
	int start_index = Path_Index(start_tx, start_ty);

	nodes[start_index].g_cost = 0;
	nodes[start_index].h_cost = Path_Heuristic(start_tx, start_ty, goal_tx, goal_ty);
	nodes[start_index].f_cost = nodes[start_index].g_cost + nodes[start_index].h_cost;

	nodes[start_index].opened = true;

	return false;
}
