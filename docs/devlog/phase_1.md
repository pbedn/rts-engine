# Phase 1 – Minimal Adjacent Movement

## Status: Complete

Phase 1 establishes the architectural foundation of the RTS project.  
The goal is not feature richness, but structural correctness.

---

# 1. Objective

Implement the smallest possible playable simulation with:

- Window creation
- Grid-based world rendering
- One controllable unit
- Adjacent tile movement only
- Smooth interpolation between tiles
- No global state
- Clear module boundaries

This phase focuses on architecture, not gameplay depth.

---

# 2. Core Architectural Principles Applied

From the project roadmap:

- Simulation > Rendering
- Deterministic-friendly structure
- Explicit data ownership
- Clear system boundaries
- No premature abstraction

---

# 3. High-Level System Structure

```
main.c
    ↓
game module
    ↓
(input) → (unit update) → (render)
    ↓
map module
```

Each system has a strict responsibility.

---

# 4. Game Loop Model

The loop follows the classical structure:

```c
while (!WindowShouldClose())
{
    float dt = GetFrameTime();

    Game_ProcessInput(&game);
    Game_Update(&game, dt);

    BeginDrawing();
    Game_Render(&game);
    EndDrawing();
}
```

Important properties:

- Input does not modify rendering.
- Rendering does not modify simulation.
- Simulation updates use delta time.
- Entire state is passed explicitly.

---

# 5. Data Ownership Model

## 5.1 GameState

GameState owns the entire simulation:

```c
typedef struct {
    Map map;
    Unit player_unit;
    float time;
} GameState;
```

No global variables exist.

All systems receive a pointer to GameState.

---

## 5.2 Map

Responsibilities:

- Store tile grid
- Define world dimensions
- Provide boundary validation
- Convert tile ↔ world coordinates

It does NOT:

- Move units
- Handle input
- Perform pathfinding

---

## 5.3 Unit

The unit is implemented as a small state machine.

It separates:

- Logical tile position (`tx`, `ty`)
- Render position (`wx`, `wy`)
- Target tile
- Movement state

```c
typedef struct {
    int tx, ty;              // Logical grid position
    float wx, wy;            // Interpolated render position
    int target_tx, target_ty;
    float speed;
    int moving;
} Unit;
```

This separation is critical for future systems.

---

# 6. Coordinate System Separation

Two coordinate spaces exist:

## 6.1 Tile Coordinates (Simulation)

```
int tx;
int ty;
```

Used for:
- Logic
- Movement rules
- Future pathfinding
- Collision
- AI decisions

---

## 6.2 World Coordinates (Rendering)

```
float wx;
float wy;
```

Used for:
- Smooth interpolation
- Visual positioning
- Animation

---

## 6.3 Conversion Responsibility

Only the Map module performs conversion:

```c
Vector2 Map_TileToWorld(int tx, int ty);
Vector2 Map_WorldToTile(float wx, float wy);
```

No direct mixing of tile and world space elsewhere.

---

# 7. Movement Model

Movement is intentionally constrained:

- Unit may move only to adjacent tile.
- Manhattan distance rule:

```c
abs(dx) + abs(dy) == 1
```

No diagonals.
No teleporting.

---

## 7.1 Movement State Machine

States:

- Idle
- Moving

When a valid adjacent tile is clicked:

1. Input module issues target.
2. Unit enters moving state.
3. Update interpolates toward target.
4. When close enough → snap to tile.
5. Movement ends.

---

## 7.2 Interpolation Logic

Movement uses normalized direction:

```c
wx += dir_x * speed * dt;
wy += dir_y * speed * dt;
```

Snap condition:

```c
if (distance < 1.0f)
```

This ensures smooth but deterministic stepping.

---

# 8. Module Responsibilities

## main.c

- Window setup
- High-level loop
- Delegation to Game module

No gameplay logic.

---

## game module

- Owns GameState
- Orchestrates subsystems
- Calls Input, Update, Render

---

## input module

- Translates mouse clicks into commands
- Validates adjacency rule
- Does NOT move unit directly

---

## unit module

- Owns movement logic
- Maintains state machine
- Handles interpolation

---

## map module

- Grid storage
- Spatial validation
- Coordinate conversions

---

## render module

- Pure visualization
- Reads simulation state
- Does not modify it

---

# 9. Determinism Considerations

Currently using variable timestep (`dt`).

Design remains deterministic-friendly because:

- All state changes happen in update phase.
- No hidden side effects.
- No randomness yet.
- No rendering-based logic.

Future improvement:

- Fixed timestep loop.

---

# 10. What Phase 1 Teaches

- Modular C project structure
- Header vs implementation separation
- Linker mechanics
- State machines
- Spatial modeling
- Coordinate system separation
- Frame-time-based interpolation
- Explicit state passing

---

# 11. Intentional Limitations

- Only one unit
- No tile blocking
- No pathfinding
- No AI
- No combat
- No selection system
- No animation system

These are intentionally deferred.

---

# 12. Architectural Validation Checklist

Phase 1 is considered complete when:

- Code compiles cleanly.
- No global variables exist.
- Unit moves only to adjacent tiles.
- Movement is smooth.
- Modules are cleanly separated.
- No circular includes.
- No “quick hacks” in main.c.

---

# 13. Phase 1 Result

A minimal but structurally correct RTS simulation core:

- Grid-based.
- Deterministic-friendly.
- Extensible.
- Architecturally clean.

This foundation enables Phase 2 (multi-tile movement and command abstraction) without refactoring core systems.
