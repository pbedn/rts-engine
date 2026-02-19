#include "input.h"
#include "raylib.h"
#include <stdlib.h>

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

        if (!Map_IsInside(&game->map, tx, ty))
            return;

        Unit* unit = &game->player_unit;

        int dx = abs(tx - unit->tx);
        int dy = abs(ty - unit->ty);

        // Adjacent rule (Manhattan distance = 1)
        if (dx + dy == 1 && !unit->moving)
        {
            Unit_SetTarget(unit, tx, ty);
        }
    }
}
