/*
    test_pathfinding.c

    Minimal test runner for Pathfinding_FindPath
    Uses real Map and Path structures.
*/

#include <stdio.h>
#include <assert.h>

#include "../src/core/map.h"
#include "../src/core/pathfinding.h"

/*
    Helper: make entire map walkable and empty.
*/
static void make_empty_map(Map *map)
{
    Map_Init(map);

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            map->tiles[y][x].walkable = 1;
            map->tiles[y][x].occupied = 0;
        }
    }
}

/*
    Test 1: straight horizontal path
*/
static void test_straight_path(void)
{
    Map map;
    make_empty_map(&map);

    Path path;

    bool found = Pathfinding_FindPath(
        &map,
        0, 0,
        3, 0,
        &path
    );

    assert(found == true);

    // Decide and lock down your invariant:
    // Does length include start tile?
    assert(path.length == 4);

    assert(path.tiles[0][0] == 0);
    assert(path.tiles[0][1] == 0);

    assert(path.tiles[path.length - 1][0] == 3);
    assert(path.tiles[path.length - 1][1] == 0);
}

/*
    Test 2: blocked goal tile
*/
static void test_blocked_goal(void)
{
    Map map;
    make_empty_map(&map);

    map.tiles[0][3].walkable = 0;

    Path path;

    bool found = Pathfinding_FindPath(
        &map,
        0, 0,
        3, 0,
        &path
    );

    assert(found == false);
}

/*
    Test 3: start equals goal
*/
static void test_same_tile(void)
{
    Map map;
    make_empty_map(&map);

    Path path;

    bool found = Pathfinding_FindPath(
        &map,
        5, 5,
        5, 5,
        &path
    );

    assert(found == true);
    assert(path.length == 1);
    assert(path.tiles[0][0] == 5);
    assert(path.tiles[0][1] == 5);
}

int main(void)
{
    printf("Running pathfinding tests...\n");

    test_straight_path();
    test_blocked_goal();
    test_same_tile();

    printf("All tests passed.\n");

    return 0;
}
