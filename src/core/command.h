#ifndef COMMAND_H
#define COMMAND_H

#include "unit.h"
#include "map.h"


// Issue a move command to a unit.
// Builds a straight-line (Manhattan) path from current tile.
// to the target tile and stores it inside the unit movement queue
void Command_MoveUnit(Unit *unit, Map *map, int target_tx, int target_ty);

#endif
