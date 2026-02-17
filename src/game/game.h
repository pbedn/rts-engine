#ifndef GAME_H
#define GAME_H

#include "../map/map.h"
#include "../units/unit.h"

typedef struct {
	Map map;
	Unit player_unit;
	float time;
} GameState;


void Game_Init(GameState* game);
void Game_ProcessInput(GameState* game);
void Game_Update(GameState* game, float dt);
void Game_Render(GameState* game);

#endif
