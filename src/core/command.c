#include "command.h"
#include "map.h"
#include "unit.h"

//Clears the unit's movement queue.
static void ClearMovementQueue(Unit *unit)
{
	unit->movement.count = 0;
	unit->movement.current_index = 0;
	unit->moving = false;
}

void Command_MoveUnit(Unit *unit, Map *map, int target_tx, int target_ty)
{
	ClearMovementQueue(unit);

	int current_tx = unit->tx;
	int current_ty = unit->ty;

	int dx = target_tx - current_tx;
	int dy = target_ty - current_ty;

	int step_x = (dx > 0) ? 1 : -1;
	int step_y = (dy > 0) ? 1 : -1;

	// Horizontal movement
	while (current_tx != target_tx) {
		current_tx += step_x;

		if (!Map_IsWalkable(map, current_tx, current_ty) ||
			Map_IsOccupied(map, current_tx, current_ty))
			break;

		if (unit->movement.count >= MAX_PATH_LENGTH)
			break;

		unit->movement.tiles[unit->movement.count][0] = current_tx;
		unit->movement.tiles[unit->movement.count][1] = current_ty;

		unit->movement.count++;
	}

	// Vertical movement
	while (current_ty != target_ty) {
		current_ty += step_y;

		if (!Map_IsWalkable(map, current_tx, current_ty))
			break;

		if (unit->movement.count >= MAX_PATH_LENGTH)
			break;

		unit->movement.tiles[unit->movement.count][0] = current_tx;
		unit->movement.tiles[unit->movement.count][1] = current_ty;

		unit->movement.count++;
	}
}
