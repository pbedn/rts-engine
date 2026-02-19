#include "unit.h"
#include <math.h>

/*
    Unit is a small state machine.

    It separates:
    - Logical tile position (tx, ty)
    - Interpolated render position (wx, wy)

    This allows smooth movement while logic remains grid-based.
*/

void Unit_Init(Unit *unit, int tx, int ty)
{
    unit->tx = tx;
    unit->ty = ty;

    // Start world position aligned with tile
    unit->wx = tx * TILE_SIZE;
    unit->wy = ty * TILE_SIZE;

    // Target initially same as current position
    unit->target_tx = tx;
    unit->target_ty = ty;

    unit->speed = 100.0f;   // Pixels per second
    unit->moving = false;

    unit->movement.count = 0;
    unit->movement.current_index = 0;
}

void Unit_SetTarget(Unit *unit, int tx, int ty)
{
    // Set new destination tile
    unit->target_tx = tx;
    unit->target_ty = ty;

    unit->moving = true;
}

void Unit_Update(Unit *unit, float dt)
{
    if (!unit->moving)
        Unit_StartNextStep(unit);

    // if moving, interpolate toward target tile
    Vector2 target = Map_TileToWorld(unit->target_tx, unit->target_ty);

    float dx = target.x - unit->wx;
    float dy = target.y - unit->wy;

    float dist = sqrtf(dx * dx + dy * dy);


    if (dist > 0.0f)
    {
        float step = unit->speed * dt;

        if (step >= dist)
        {
            // Snap to target
            unit->wx = target.x;
            unit->wy = target.y;

            // Commit tile position
            unit->tx = unit->target_tx;
            unit->ty = unit->target_ty;

            unit->moving = false;

            // Advance movement queue
            unit->movement.current_index++;
        }
        else
        {
            // Normalize direction
            float dir_x = dx / dist;
            float dir_y = dy / dist;

            // Move toward target using frame time
            unit->wx += dir_x * step;
            unit->wy += dir_y * step;
        }
    }
}

// Starts movement toward the next tile in the queue if available
// Returns true if movement started, false otherwise
static bool Unit_StartNextStep(Unit *unit)
{
    if (unit->movement.current_index >= unit->movement.count)
        return false;

    int next_tx = unit->movement.tiles[unit->movement.current_index][0];
    int next_ty = unit->movement.tiles[unit->movement.current_index][1];

    unit->target_tx = next_tx;
    unit->target_ty = next_ty;

    unit->moving = true;

    return true;
}