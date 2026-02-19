/*
Pathfinding module.

Implements A* pathfinding on the grid-based Map.

This module:
- Reads Map data
- Does not modify Map
- Does not allocate memory dynamically
- Is fully deterministic
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "pathfinding.h"
#include "map.h"
#include "unit.h"

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
	int f_cost;  // g + h

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

/*
Returns index of the opened node with the lowest f_cost.
If none found, returns -1.

Linear scan — simple and deterministic.
*/
static int Path_FindLowestCost(PathNode *nodes)
{
    int best_index = -1;

    for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; ++i)
    {
        if (!nodes[i].opened)
            continue;

        if (nodes[i].closed)
            continue;

        if (best_index == -1 ||
            nodes[i].f_cost < nodes[best_index].f_cost)
        {
            best_index = i;
        }
    }

    return best_index;
}

/*
Finds a path between start and goal tile coordinates.

Returns:
    true  -> path found, out_path is filled
    false -> no path possible

Rules:
- Does not allocate memory
- Does not modify Map
- Deterministic behavior
*/
bool Pathfinding_FindPath(
	const Map *map,
	int start_tx,
	int start_ty,
	int goal_tx,
	int goal_ty,
	Path *out_path
)
{
	printf("-- Pathfinding start --\n");
	printf("Start: (%d,%d)\n", start_tx, start_ty);
	printf("Goal:  (%d,%d)\n", goal_tx, goal_ty);
	printf("Goal walkable: %d\n", Map_IsWalkable(map, goal_tx, goal_ty));
	printf("Goal occupied: %d\n", Map_IsOccupied(map, goal_tx, goal_ty));
	fflush(stdout);

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

			nodes[index].parent_index = -1;

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

	// --- A* Main Loop ---
	while(1)
	{
		int current_index = Path_FindLowestCost(nodes);

		// no open nodes left - no path
		if (current_index == -1)
			return false;

		PathNode *current = &nodes[current_index];

		// if goal reached - stop search
		if (current->tx == goal_tx && current->ty == goal_ty)
			break;

		current->opened = false;
		current->closed = true;

		// neigbour offsets (Up, right, down, Left)
		const int offsets[4][2] = 
		{
			{ 0, -1 },
			{ 1,  0 },
			{ 0,  1 },
			{ -1, 0 }
		};

		for (int i = 0; i < 4; ++i)
		{
			int nx = current->tx + offsets[i][0];
			int ny = current->ty + offsets[i][1];

			if (!Map_IsInside(map, nx, ny))	
				continue;

			if (!Map_IsWalkable(map, nx, ny))
				continue;

			if (Map_IsOccupied(map, nx, ny))
			    continue;

			int neigbhour_index = Path_Index(nx, ny);
			PathNode *neighbour = &nodes[neigbhour_index];

			if (neighbour->closed)
				continue;

			int tentative_g = current->g_cost + 1;

			if (!neighbour->opened || tentative_g < neighbour->g_cost)
			{
				neighbour->g_cost = tentative_g;
				neighbour->h_cost = Path_Heuristic(nx, ny, goal_tx, goal_ty);
				neighbour->f_cost = neighbour->g_cost + neighbour->h_cost;

				neighbour->parent_index = current_index;
				neighbour->opened = true;
			}
		}
	}

	// --- Path Reconstruction ---

	// Goal node index
	int goal_index = Path_Index(goal_tx, goal_ty);

	int path_indices[MAX_PATH_LENGTH];
	int path_length = 0;

	int current_index = goal_index;

	// Walk backwards from goal to start
	// goal -> parent -> parent -> ... -> start
	while (current_index != -1)
	{
		if (path_length >= MAX_PATH_LENGTH)
			// Path too long for buffer
			return false;

		path_indices[path_length++] = current_index;
		current_index = nodes[current_index].parent_index;
	}

	// Reverse order (start -> ... -> goal)
	for (int i = 0; i < path_length; ++i)
	{
		int reversed_index = path_indices[path_length - 1 - i];
		PathNode *node = &nodes[reversed_index];

		out_path->tiles[i][0] = node->tx;
		out_path->tiles[i][1] = node->ty;
	}

	out_path->length = path_length;

	return true;
}
