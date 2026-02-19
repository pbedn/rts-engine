# RTS Engine – C Style Guide

## 1. Core Principles

- Clarity over cleverness
- Explicit over implicit
- Deterministic behavior over convenience
- Small, well-defined modules
- Minimize global visibility

---

## 2. File & Module Structure

Each module consists of:

    module.h   → public API
    module.c   → implementation

### Rules

- Headers contain **declarations only**
- Source files contain **definitions**
- Every `.c` file includes its own header first
- No cross-module includes in headers unless strictly required

---

## 3. Naming Conventions

### Types

```c
typedef struct Unit Unit;
typedef struct Map  Map;
```

- PascalCase for structs and typedefs
- No `_t` suffix

---

### Functions

Public API:

```c
void Map_Update(Map *map);
void Command_MoveUnit(Unit *unit, Map *map);
```

Private (file-local):

```c
static void recalc_tiles(Map *map);
```

Rules:
- `Module_ActionObject` naming
- Verb-first for behavior
- Use `static` for all non-API functions

---

### Variables

```c
int tile_count;
float delta_time;
Unit *unit;
```

Rules:
- snake_case
- Descriptive names
- No abbreviations unless standard

---

## 4. Pointer Style

Use:

```c
Type *name;
```

Never:

```c
Type* name;
```

Reason:
- `*` binds to the variable, not the type
- Avoids multi-declaration confusion

Correct:

```c
Unit *a, *b;
```

---

## 5. Const Correctness

Use `const` aggressively for read-only data:

```c
void Render_Draw(const Map *map);
```

Rules:
- Inputs are `const` unless mutated
- Protect module boundaries
- Prevent accidental writes

---

## 6. Static Usage

Use `static` for:
- Private functions in `.c`
- Internal file-level globals

Never use `static` for:
- Public API functions
- Large implementations in headers

Allowed in headers:

```c
static inline int clamp(int x, int min, int max);
```

---

## 7. Headers

Every header:

```c
#ifndef MAP_H
#define MAP_H

// declarations only

#endif
```

Rules:
- Include guards mandatory
- No function bodies (except small `static inline`)
- No global variable definitions

---

## 8. Error Handling

- No silent failures
- Validate inputs
- Avoid `atoi` → use `strtol`
- Fail early in development builds

---

## 9. Memory Rules

- Ownership must be clear
- The allocator frees
- No hidden allocation in “getter” functions
- No mutation through ambiguous references

---

## 10. No Hidden State

Avoid:
- Global mutable variables
- Implicit cross-module coupling

Prefer:
- Explicit parameter passing
- Clear state containers (`GameState`, `Map`, etc.)

---

## 11. Function Design

Good function:
- Does one thing
- Has clear input/output
- No hidden side effects
- < ~50 lines (soft rule)

Avoid:
- Deep nesting
- Hidden mutation
- Mixed responsibilities

---

## 12. Determinism

RTS simulation must be:
- Order-stable
- Floating-point aware
- Update-phase separated from render-phase

Never mix:

```c
Render_Draw();
```

with:

```c
Simulation_Update();
```

---

## 13. Comments

Comment:
- Why something exists
- Invariants
- Assumptions

Do not comment:
- Obvious syntax
- Redundant explanations

Example:

```c
// Units must not move during iteration of unit list
```

---

## 14. Formatting

- 4 spaces indentation
- No tabs
- One statement per line
- Braces on new line

```c
if (condition)
{
    ...
}
```

---

## 15. Architectural Discipline

- Core simulation independent from rendering
- No Raylib calls inside game logic
- Rendering layer reads state, never mutates it
- Input translates into commands, not direct mutation

---

# Guiding Rule

If something feels implicit, hidden, or magical — redesign it.

The engine must remain readable months later without guessing intent.
