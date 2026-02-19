#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "map.h"
#include "unit.h"

typedef struct {
	Map map;
	Unit player_unit;
	float time;
} GameState;

#endif
