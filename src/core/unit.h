#ifndef UNIT_H
#define UNIT_H

#include "raylib.h"
#include "map.h"

// Maximum number of tiles a unit can have in its movement queue
// Fixed-size to avoid dynamic allocation at this stage
#define MAX_PATH_LENGTH 128

typedef struct
{
	// Array of tile coordinates [tx, ty]
	int tiles[MAX_PATH_LENGTH][2];

	// Number of valid tiles stored in the queue
	int count;

	// Index of the next tile unit should move toward
	int current_index;
} MovementQueue;


typedef struct {
	int id;

	// Logical position (simulation)
	int tx;
	int ty;

	// Render position (interpolated)
	float wx;
	float wy;

	// Target tile of the current interpolation step
	int target_tx;
	int target_ty;

	// Movement properties
	float speed;
	// Indicates wheter the unit is currently interpolating toward a tile
	bool moving;

	// MovementQueue containing future tile steps
	MovementQueue movement;

} Unit;

void Unit_Init(Unit *unit, Map *map, int tx, int ty);
void Unit_Update(Unit *unit, Map *map, float dt);
static bool Unit_StartNextStep(Unit *unit);

#endif
