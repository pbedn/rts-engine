#include "render.h"
#include "raylib.h"

/*
    Render module visualizes current simulation state.

    It reads from GameState.
    It does NOT modify simulation.
*/

void Render_Draw(GameState* game)
{
    // Draw tile grid
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            DrawRectangleLines(
                x * TILE_SIZE,
                y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE,
                LIGHTGRAY
            );
        }
    }

    // Draw unit
    Unit* u = &game->player_unit;

    DrawCircle(
        u->wx + TILE_SIZE / 2,
        u->wy + TILE_SIZE / 2,
        TILE_SIZE / 3,
        RED
    );
}
