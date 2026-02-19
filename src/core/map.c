#include "map.h"

/*
    Map module owns spatial grid.
    It provides:
    - Initialization
    - Boundary checks
    - Coordinate conversions

    It does NOT:
    - Move units
    - Handle input
*/

void Map_Init(Map *map)
{
    // Initialize all tiles as walkable
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            map->tiles[y][x].walkable = 1;
            map->tiles[y][x].occupied = 0;
        }
    }
}

bool Map_IsInside(const Map *map, int tx, int ty)
{
    // map currently unused, but kept for future flexibility
    (void)map;

    return tx >= 0 && tx < MAP_WIDTH &&
           ty >= 0 && ty < MAP_HEIGHT;
}

bool Map_IsWalkable(const Map *map, int tx, int ty)
{
    if (!Map_IsInside(map, tx, ty))
    {
        return false;
    }

    // for phase 2: everything is walkable
    return map->tiles[ty][tx].walkable != 0;
}

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

bool Map_IsOccupied(const Map *map, int tx, int ty)
{
    if (!Map_IsInside(map, tx, ty))
        return true;

    return map->tiles[ty][tx].occupied != 0;
}

void Map_SetOccupied(Map *map, int tx, int ty, bool value)
{
    if (!Map_IsInside(map, tx, ty))
        return;

    map->tiles[ty][tx].occupied = value ? 1: 0;
}
