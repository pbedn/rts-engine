#include "gamestate.h"

bool GameState_CanIssueMove(const GameState *game, int target_tx, int target_ty)
{
    if (!Map_IsInside(&game->map, target_tx, target_ty))
    {
        return false;
    }

    if (!Map_IsWalkable(&game->map, target_tx, target_ty))
    {
        return false;
    }

    if (Map_IsOccupied(&game->map, target_tx, target_ty))
    {
        return false;
    }

    return true;
}
