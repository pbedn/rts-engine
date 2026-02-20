#include "raylib.h"
#include "input.h"
#include "../core/command.h"
#include "../core/gamestate.h"
#include "../core/coords.h"


/*
    Input module interprets user intent.

    It does NOT:
    - Move units directly
    - Render anything

    It issues commands (set target).
*/

void Input_Process(GameState *game)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Vector2 mouse = GetMousePosition();
        Vector2 tile = Map_WorldToTile(mouse.x, mouse.y);

        game->input.has_move_order = true;
        game->input.move_tx = (int)tile.x;
        game->input.move_ty = (int)tile.y;
    }

    if (IsKeyPressed(KEY_F1))
    {
        game->debug_draw_pathfinding = !game->debug_draw_pathfinding;
    }
}
