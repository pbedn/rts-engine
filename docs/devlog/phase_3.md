# Phase 3: A* Pathfinding

## 1. Purpose of Phase 3

Phase 3 introduces real pathfinding using the A* algorithm.

Until Phase 2, movement was scripted:

* Horizontal stepping
* Then vertical stepping
* No awareness of obstacles beyond immediate blocking

Phase 3 replaces that with:

* Graph-based navigation
* Shortest-path computation
* Terrain-aware routing
* Deterministic algorithmic behavior

This is the first true algorithm-heavy system in the engine.

---

## 2. Architectural Placement

Pathfinding is part of **core simulation logic**.

It:

* Reads `Map`
* Does not modify `Map`
* Does not depend on rendering
* Does not depend on input
* Does not allocate dynamic memory

### File Location

```
src/core/pathfinding.h
src/core/pathfinding.c
```

This respects the architectural rule:

> Simulation systems must not depend on rendering or input layers.

---

## 3. Design Goals

The implementation intentionally favors:

* Clarity over optimization
* Determinism over performance tricks
* Fixed memory over dynamic allocation
* Simple linear scans over heaps
* Explicit state over hidden structures

No priority queue.
No dynamic resizing.
No pointer graphs.

Everything is explicit and readable.

---

## 4. World as an Implicit Graph

We do **not** store a graph structure.

Instead:

* Each tile = a node
* Each walkable adjacent tile = an edge
* Edge cost = 1
* Graph is derived from `Map`

### Node Indexing Strategy

Tiles are mapped to a linear array using:

```c
static int Path_Index(int tx, int ty)
{
    return ty * MAP_WIDTH + tx;
}
```

This ensures:

* Deterministic memory layout
* No pointer-based graph
* Easy debugging

---

## 5. PathNode Structure

Internal-only representation:

```c
typedef struct
{
    int tx;
    int ty;

    int g_cost;        // Cost from start node
    int h_cost;        // Heuristic to goal
    int f_cost;        // g + h

    int parent_index;  // Index of previous node in path

    bool opened;
    bool closed;

} PathNode;
```

### Design Rationale

* Parent stored as index (not pointer)
* No dynamic allocation
* One node per tile
* Flags instead of separate open/closed lists

---

## 6. Heuristic Choice

We use Manhattan distance:

```c
static int Path_Heuristic(int ax, int ay, int bx, int by)
{
    return abs(ax - bx) + abs(ay - by);
}
```

Reasons:

* Grid is 4-directional
* Movement cost per tile = 1
* Manhattan is admissible
* Integer-only math
* Deterministic

No floating point.

---

## 7. Open Set Selection

We use a linear scan:

```c
static int Path_FindLowestCost(PathNode *nodes)
{
    int best_index = -1;

    for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; ++i)
    {
        if (!nodes[i].opened)
            continue;

        if (nodes[i].closed)
            continue;

        if (best_index == -1 ||
            nodes[i].f_cost < nodes[best_index].f_cost)
        {
            best_index = i;
        }
    }

    return best_index;
}
```

### Why Not a Heap?

Because:

* Map size is small
* Clarity > performance
* Linear scan is easy to reason about
* No additional data structures required
* Easier debugging

Optimization can come later if needed.

---

## 8. Neighbor Expansion Order

Neighbors are processed in fixed order:

```c
const int offsets[4][2] =
{
    { 0, -1 },  // Up
    { 1,  0 },  // Right
    { 0,  1 },  // Down
    { -1, 0 }   // Left
};
```

This ensures:

* Deterministic traversal
* Stable tie-breaking
* Predictable path shapes

---

## 9. Obstacle Handling

A tile is considered valid if:

```c
if (!Map_IsInside(map, nx, ny))
    continue;

if (!Map_IsWalkable(map, nx, ny))
    continue;

if (Map_IsOccupied(map, nx, ny))
    continue;
```

This integrates:

* Terrain blocking
* Unit blocking

Pathfinding respects simulation state without modifying it.

---

## 10. Path Reconstruction

After reaching the goal:

1. Follow `parent_index` backward.
2. Store indices in temporary array.
3. Reverse into output path.

```c
int path_indices[MAX_PATH_LENGTH];
int path_length = 0;

int current_index = goal_index;

while (current_index != -1)
{
    path_indices[path_length++] = current_index;
    current_index = nodes[current_index].parent_index;
}

for (int i = 0; i < path_length; ++i)
{
    int reversed_index = path_indices[path_length - 1 - i];
    PathNode *node = &nodes[reversed_index];

    out_path->tiles[i][0] = node->tx;
    out_path->tiles[i][1] = node->ty;
}
```

The path includes:

* Start tile
* Goal tile

---

## 11. Public API

Defined in `pathfinding.h`:

```c
typedef struct
{
    int tiles[MAX_PATH_LENGTH][2];
    int length;
} Path;

bool Pathfinding_FindPath(
    const Map *map,
    int start_tx,
    int start_ty,
    int goal_tx,
    int goal_ty,
    Path *out_path
);
```

### Ownership Rules

* Caller owns `Path`
* No dynamic memory allocation
* No hidden state
* No global variables

---

## 12. Integration With Command System

`Command_MoveUnit` now:

1. Clears movement queue.
2. Calls `Pathfinding_FindPath`.
3. Copies tiles (skipping index 0).
4. Starts movement.

```c
Path path;

bool found = Pathfinding_FindPath(
    map,
    unit->tx,
    unit->ty,
    target_tx,
    target_ty,
    &path
);

for (int i = 1; i < path.length; ++i)
{
    unit->movement.tiles[unit->movement.count][0] = path.tiles[i][0];
    unit->movement.tiles[unit->movement.count][1] = path.tiles[i][1];
    unit->movement.count++;
}
```

Separation of responsibilities:

* Input → issues command
* Command → generates path
* Pathfinding → pure algorithm
* Unit → executes movement

---

## 13. Determinism Guarantees

The implementation is deterministic because:

* No floating point in algorithm
* No randomness
* Fixed neighbor order
* Linear open set scan
* Explicit memory layout

This prepares for:

* Replay system
* Lockstep multiplayer (future phase)

---

## 14. Known Constraints (Intentional)

* O(n²) open-set scan
* No diagonal movement
* No path smoothing
* No dynamic re-pathing
* Fixed path buffer size

These are acceptable trade-offs for learning clarity.

---

## 15. Learning Outcomes

Phase 3 teaches:

* Graph modeling from grid
* Cost propagation
* Heuristic design
* Parent tracking
* Memory-safe C implementation
* Debugging algorithmic bugs
* Deterministic simulation design

This is the first true algorithmic backbone of the RTS engine.

---

## 16. Phase 3 Completion Criteria

Phase 3 is complete when:

* Unit routes around obstacles
* Unit respects terrain blocking
* Unit respects occupancy
* No crashes
* No dynamic allocation
* Clean compilation with no warnings
