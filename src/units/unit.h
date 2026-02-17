#ifndef UNIT_H
#define UNIT_H

#include "raylib.h"
#include "../map/map.h"

typedef struct {
	// Logical position (simulation)
	int tx;
	int ty;

	// Render postion (interpolated)
	float wx;
	float wy;

	// Movement target
	int target_tx;
	int target_ty;

	// Movement properties
	float speed;
	int moving;
} Unit;

void Unit_Init(Unit* unit, int tx, int ty);
void Unit_SetTarget(Unit* unit, int tx, int ty);
void Unit_Update(Unit* unit, float dt);

#endif
