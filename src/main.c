#include <stdio.h>
#include "game/game.h"
#include "raylib.h"
#include "palette.h"

/*
    Entry point.

    main.c is intentionally thin.
    It owns:
    - Window creation
    - High-level game loop
    - Delegation to Game module

    It does NOT:
    - Contain gameplay logic
    - Move units
    - Render directly
*/

int main(void)
{
    // Window size derived from map dimensions.
    const int screenWidth  = MAP_WIDTH * TILE_SIZE;
    const int screenHeight = MAP_HEIGHT * TILE_SIZE;

    SetConfigFlags(FLAG_WINDOW_HIGHDPI | FLAG_VSYNC_HINT);

    InitWindow(screenWidth, screenHeight, "RTS Project");
    // printf("Window: %d x %d\n", GetScreenWidth(), GetScreenHeight());
    // printf("Render: %d x %d\n", GetRenderWidth(), GetRenderHeight());
    // printf("Monitor: %d x %d\n", GetMonitorWidth(0), GetMonitorHeight(0));

    // Entire simulation state lives here.
    GameState game;
    Game_Init(&game);

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        float dt = GetFrameTime();  // Time since last frame (seconds)

        Game_ProcessInput(&game);
        Game_Update(&game, dt);

        BeginDrawing();
        ClearBackground(PALETTE_BACKGROUND);
        
        Game_Render(&game);
        
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
