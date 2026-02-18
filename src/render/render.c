#include "render.h"
#include "raylib.h"
#include "palette.h"
#include <stdio.h>

/*
    Render module visualizes current simulation state.

    It reads from GameState.
    It does NOT modify simulation.
*/

void Render_Draw(GameState* game)
{
    bool render_debug_show_coordinates = true;

    // Draw tile grid
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            int world_x = x * TILE_SIZE;
            int world_y = y * TILE_SIZE;

            DrawRectangleLines(
                world_x,
                world_y,
                TILE_SIZE,
                TILE_SIZE,
                PALETTE_GRID
            );
            if (render_debug_show_coordinates)
            {
                RenderTileCoordinates(x, y, world_x, world_y, TILE_SIZE);
            }
        }
    }

    // Draw unit
    Unit* u = &game->player_unit;

    DrawCircle(
        u->wx + TILE_SIZE / 2,
        u->wy + TILE_SIZE / 2,
        TILE_SIZE / 3,
        PALETTE_UNIT
    );
}

static void RenderTileCoordinates(int tx, int ty, int wx, int wy, int tile_size)
{
    char buffer[16];

    snprintf(buffer, sizeof(buffer), "%d,%d", tx, ty);

    int font_size = tile_size / 5;
    if (font_size < 8)
        font_size = 8;

    int text_width = MeasureText(buffer, font_size);
    int text_x = wx + (tile_size - text_width) / 2;
    int text_y = wy + (tile_size - font_size) / 2;

    DrawText(buffer, text_x, text_y, font_size, PALETTE_COORD_TEXT);
}
