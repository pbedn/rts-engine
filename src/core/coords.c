#include "raylib.h"

#include "../game/constants.h"

/*
    Converts discrete tile coordinate to pixel space.
    This is required to separate logic from rendering.
*/
Vector2 Map_TileToWorld(int tx, int ty)
{
    Vector2 pos;
    pos.x = tx * TILE_SIZE;
    pos.y = ty * TILE_SIZE;
    return pos;
}

/*
    Converts pixel space into tile coordinate.
    Used for mouse click interpretation.
*/
Vector2 Map_WorldToTile(float wx, float wy)
{
    Vector2 tile;
    tile.x = (int)(wx / TILE_SIZE);
    tile.y = (int)(wy / TILE_SIZE);
    return tile;
}
