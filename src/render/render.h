#ifndef RENDER_H
#define RENDER_H

#include "raylib.h"
#include "../core/gamestate.h"

void Render_Draw(GameState *game);

Vector2 Map_TileToWorld(int tx, int ty);
Vector2 Map_WorldToTile(float wx, float wy);

#endif
