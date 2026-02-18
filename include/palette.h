/*
    palette.h

    Centralized color palette for the RTS project.

    Design goals:
    - Single-header solution (no .c file required)
    - No macros
    - No global mutable state
    - Safe to include in multiple translation units

    Implementation detail:
    We use `static const` so each translation unit gets
    its own internal copy, avoiding multiple definition errors.
*/

#ifndef PALETTE_H
#define PALETTE_H

#include "raylib.h"

/*
    Warm, low-blue-light night palette.
    All colors are fully opaque (alpha = 255).
*/

static const Color PALETTE_BACKGROUND    = {  30,  26,  23, 255 };  // #1E1A17
static const Color PALETTE_GRID          = {  63,  56,  51, 255 };  // #3F3833
static const Color PALETTE_TILE_WALKABLE = {  79,  91,  58, 255 };  // #4F5B3A
static const Color PALETTE_TILE_BLOCKED  = { 107,  78,  61, 255 };  // #6B4E3D
static const Color PALETTE_UNIT          = { 196, 106,  47, 255 };  // #C46A2F
static const Color PALETTE_SELECTION     = { 230, 184,  92, 255 };  // #E6B85C

/*
    Muted warm gray.
    Slight transparency (alpha 180) so it does not dominate.
*/
static const Color PALETTE_COORD_TEXT = { 120, 110, 100, 180 };


#endif
