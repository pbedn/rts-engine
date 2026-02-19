#include <stdio.h>
#include "command.h"
#include "map.h"
#include "raylib.h"
#include "unit.h"
#include "pathfinding.h"


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

	// Validate target first
    if (!Map_IsInside(map, target_tx, target_ty))
        return;

    if (!Map_IsWalkable(map, target_tx, target_ty))
        return;

    if (Map_IsOccupied(map, target_tx, target_ty))
        return;

    Path path;

    bool found = Pathfinding_FindPath(map, unit->tx, unit->ty, target_tx, target_ty, &path);

    if (!found)
    	return;

    // Copy path into movement queue
    // Skip index 0 because that is the unit's current tile

    for (int i = 1; i < path.length; ++i)
    {
    	if (unit->movement.count >= MAX_PATH_LENGTH)
    		break;

    	unit->movement.tiles[unit->movement.count][0] = path.tiles[i][0];
    	unit->movement.tiles[unit->movement.count][1] = path.tiles[i][1];

    	unit->movement.count++;
    }

    TraceLog(LOG_INFO, "Path length: %d", path.length);
}
