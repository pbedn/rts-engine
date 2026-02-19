#include <stdio.h>
#include "raylib.h"
#include "input.h"
#include "../core/command.h"


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
        Vector2 tile  = Map_WorldToTile(mouse.x, mouse.y);

        int tx = (int)tile.x;
        int ty = (int)tile.y;

        Command_MoveUnit(&game->player_unit, &game->map, tx, ty);
    }
}
