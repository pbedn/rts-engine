#include "unit.h"
#include <math.h>

/*
    Unit is a small state machine.

    It separates:
    - Logical tile position (tx, ty)
    - Interpolated render position (wx, wy)

    This allows smooth movement while logic remains grid-based.
*/

void Unit_Init(Unit* unit, int tx, int ty)
{
    unit->tx = tx;
    unit->ty = ty;

    // Start world position aligned with tile
    unit->wx = tx * TILE_SIZE;
    unit->wy = ty * TILE_SIZE;

    // Target initially same as current position
    unit->target_tx = tx;
    unit->target_ty = ty;

    unit->speed = 150.0f;   // Pixels per second
    unit->moving = 0;
}

void Unit_SetTarget(Unit* unit, int tx, int ty)
{
    // Set new destination tile
    unit->target_tx = tx;
    unit->target_ty = ty;

    unit->moving = 1;
}

void Unit_Update(Unit* unit, float dt)
{
    if (!unit->moving)
        return;

    Vector2 target = Map_TileToWorld(unit->target_tx, unit->target_ty);

    float dx = target.x - unit->wx;
    float dy = target.y - unit->wy;

    float dist = sqrtf(dx * dx + dy * dy);

    // If close enough → snap and finish movement
    if (dist < 1.0f)
    {
        unit->wx = target.x;
        unit->wy = target.y;

        unit->tx = unit->target_tx;
        unit->ty = unit->target_ty;

        unit->moving = 0;
        return;
    }

    // Normalize direction
    float dir_x = dx / dist;
    float dir_y = dy / dist;

    // Move toward target using frame time
    unit->wx += dir_x * unit->speed * dt;
    unit->wy += dir_y * unit->speed * dt;
}
