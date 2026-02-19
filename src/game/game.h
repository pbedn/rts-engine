#ifndef GAME_H
#define GAME_H

#include "../core/gamestate.h"

void Game_Init(GameState* game);
void Game_ProcessInput(GameState* game);
void Game_Update(GameState* game, float dt);
void Game_Render(GameState* game);

#endif
