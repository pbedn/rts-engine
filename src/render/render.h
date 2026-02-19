#ifndef RENDER_H
#define RENDER_H

#include "../core/gamestate.h"

void Render_Draw(GameState* game);
static void RenderTileCoordinates(int tx, int ty, int wx, int wy, int tile_size);

#endif
