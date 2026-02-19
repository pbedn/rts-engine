# Phase 2 – Multi-Tile Movement & Tile Blocking

## 1. Phase Goal

Extend the Phase 1 single-tile movement into a proper RTS-style movement system by introducing:

- Movement queue
- Command abstraction layer
- Straight-line (Manhattan) path building
- Tile occupancy (blocking)
- Clear separation between input, command, simulation, and map systems

This phase transitions the project from “toy movement” to a structured simulation architecture.

---

## 2. Architectural Shift from Phase 1

### Phase 1

Input directly modified the unit:

```
click → Unit_SetTarget() → interpolate → done
```

Problems:
- Tight coupling
- No abstraction of intent
- No extensibility
- No path planning

---

### Phase 2

Input no longer mutates the unit directly.

```
click → Command_MoveUnit()
       → build path
       → store queue in unit
       → simulation consumes queue
```

Key idea:

> Input creates intent.  
> Command converts intent into data.  
> Simulation consumes data.

This separation is foundational for RTS design.

---

## 3. Systems Modified in Phase 2

### 3.1 Unit System

#### New: MovementQueue

```c
#define MAX_PATH_LENGTH 128

typedef struct
{
    int tiles[MAX_PATH_LENGTH][2];
    int count;
    int current_index;
} MovementQueue;
```

Purpose:
- Store precomputed tile steps
- Avoid dynamic allocation
- Keep deterministic memory usage

---

#### Unit Struct Changes

Added:

- `MovementQueue movement`
- `bool is_moving`
- `int target_tx`
- `int target_ty`

Important distinction:

| Field               | Meaning |
|---------------------|----------|
| tile_x / tile_y     | Committed tile (logical grid) |
| world_x / world_y   | Interpolated render position |
| target_tx / target_ty | Tile currently moving toward |
| movement queue      | Future tile steps |

---

#### Unit_Update Redesign

Unit became a small internal state machine:

1. If not moving → start next queued tile
2. If moving → interpolate
3. If reached → commit tile, advance queue
4. Repeat until queue empty

Unit no longer builds paths.  
Unit no longer reads input.  
It only executes.

---

### 3.2 Command System (New Module)

Created:

```
command.h
command.c
```

Primary API:

```c
void Command_MoveUnit(Unit *unit, Map *map, int target_tx, int target_ty);
```

Responsibilities:

- Clear previous movement queue
- Build straight-line path (Manhattan)
- Store steps inside unit

Command layer:
- Knows Unit
- Knows Map
- Does not know Input
- Does not know Rendering

This enforces clean layering.

---

### 3.3 Path Builder (Temporary)

Straight-line Manhattan path:

1. Move horizontally toward target
2. Move vertically toward target
3. Stop if:
   - Tile not walkable
   - Tile occupied
   - Path length limit reached

No diagonals.  
No A*.  
No heuristics.

This is intentionally simple.

Phase 3 will replace only the path builder.

---

### 3.4 Map System Extension

Original Map:

```c
typedef struct {
    int walkable;
} Tile;
```

Extended to:

```c
typedef struct {
    int walkable;
    int occupied;
} Tile;
```

Map now owns:

- Static terrain (`walkable`)
- Dynamic state (`occupied`)

---

#### New Map API

```c
bool Map_IsInside(const Map *map, int tx, int ty);
bool Map_IsWalkable(const Map *map, int tx, int ty);
bool Map_IsOccupied(const Map *map, int tx, int ty);
void Map_SetOccupied(Map *map, int tx, int ty, bool value);
```

Important design decision:

- Walkability = terrain
- Occupancy = dynamic runtime state
- They are not merged

This preserves conceptual clarity.

---

### 3.5 Occupancy Handling

Integrated into movement lifecycle:

- `Unit_Init()` → claim starting tile
- `Command_MoveUnit()` → avoid occupied tiles
- `Unit_Update()` (on step commit):
  - Release old tile
  - Claim new tile

Now:

- Units cannot overlap
- Spatial rules are enforced
- Map owns grid authority

---

## 4. Functions Removed

### `Unit_SetTarget()`

Removed intentionally.

Reason:
- Direct mutation violates command abstraction
- Input must not bypass command layer

This enforces architectural discipline.

---

## 5. System Flow After Phase 2

```
process_input()
    → Command_MoveUnit()

update_simulation(dt)
    → Unit_Update(unit, map, dt)

render()
    → read world_x / world_y only
```

Clear separation of concerns.

---

## 6. What Phase 2 Achieved

### Structural Improvements

- Introduced command abstraction
- Introduced internal unit state machine
- Introduced tile blocking
- Separated terrain and occupancy
- Removed direct movement mutation
- Preserved deterministic logic

---

### Architectural Principles Reinforced

- Simulation > Rendering
- Clear module boundaries
- Explicit state ownership
- No hidden global state
- No premature optimization
- Fixed memory footprint

---

## 7. Limitations (Intentional)

- No A* pathfinding
- No multiple units coordination
- No command stacking
- No re-routing
- No dynamic memory
- No diagonal movement

All intentional.

This phase is about structure, not features.

---

## 8. Learning Outcomes

Phase 2 reinforced:

- Abstraction boundaries
- State machines
- Grid reasoning
- Data ownership
- Incremental architecture
- Avoiding premature complexity

It is the first step toward a true RTS simulation.

---

## 9. Phase 2 Completion Criteria

✔ Multi-tile movement works  
✔ Movement queue drives behavior  
✔ Command layer owns path building  
✔ Map enforces occupancy  
✔ No direct unit mutation from input  
✔ Clean compilation with no hacks  

Phase 2 is architecturally sound.
