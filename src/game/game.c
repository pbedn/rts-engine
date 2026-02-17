#include "game.h"
#include "../input/input.h"
#include "../render/render.h"

/*
    Game module orchestrates subsystems.

    It owns:
    - Map
    - Units
    - Global time

    It delegates:
    - Input handling
    - Rendering
*/

void Game_Init(GameState* game)
{
    Map_Init(&game->map);

    // Create single test unit in middle of map
    Unit_Init(&game->player_unit, 5, 5);

    game->time = 0.0f;
}

void Game_ProcessInput(GameState* game)
{
    // Input modifies simulation state via commands
    Input_Process(game);
}

void Game_Update(GameState* game, float dt)
{
    // Advance global time
    game->time += dt;

    // Update simulation objects
    Unit_Update(&game->player_unit, dt);
}

void Game_Render(GameState* game)
{
    // Rendering is delegated to render module
    Render_Draw(game);
}
