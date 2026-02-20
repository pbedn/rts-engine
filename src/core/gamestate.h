#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <stdbool.h>
#include "map.h"
#include "unit.h"
#include "pathfinding.h"

typedef struct {
	bool has_move_order;
	int move_tx;
	int move_ty;
} PendingInput;

typedef struct {
	Map map;
	Unit player_unit;
	float time;

	// debug pathfinding
	Path debug_last_path;
	bool debug_draw_pathfinding;

	PendingInput input;
} GameState;

// Command-policy check owned by game state.
// Determines whether a move command target is currently valid.
bool GameState_CanIssueMove(const GameState *game, int target_tx, int target_ty);

#endif
