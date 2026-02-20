#include "../core/gamestate.h"
#include "../input/input.h"
#include "../render/render.h"
#include "../core/command.h"

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

void Game_Init(GameState *game)
{
    Map_Init(&game->map);

    // Create single test unit in middle of map
    Unit_Init(&game->player_unit, &game->map, 5, 5);

    // Just for testing, injecting path manually
    // game->player_unit.movement.tiles[0][0] = 5;
    // game->player_unit.movement.tiles[0][1] = 3;

    // game->player_unit.movement.tiles[1][0] = 6;
    // game->player_unit.movement.tiles[1][1] = 3;

    // game->player_unit.movement.count = 2;
    // game->player_unit.movement.current_index = 0;


    game->time = 0.0f;

    game->debug_draw_pathfinding = false;
    game->debug_last_path = (Path){0};
}

void Game_ProcessInput(GameState *game)
{
    // Input modifies simulation state via commands
    Input_Process(game);
}

void Game_Update(GameState *game, float dt)
{
    // Advance global time
    game->time += dt;

    if (game->input.has_move_order)
    {
        Command_MoveUnit(
            &game->player_unit,
            &game->map,
            game->input.move_tx,
            game->input.move_ty,
            &game->debug_last_path
        );
        game->input.has_move_order = false;
    }

    // Update simulation objects
    Unit_Update(&game->player_unit, &game->map, dt);
}

void Game_Render(GameState *game)
{
    // Rendering is delegated to render module
    Render_Draw(game);

    Render_DrawPathDebug(game);
}
