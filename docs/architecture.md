# RTS Project – Architecture & Learning Roadmap

(Raylib + C, Grid-Based Simulation)

## 0. Project Philosophy

Goals
- Learn systems thinking
- Learn C deeply
- Build architecture consciously
- Avoid copy–paste coding
- Prefer clarity over speed

Non-Goals
- No production-ready polish
- No engine-like abstraction early
- No premature optimization
- No feature explosion

## 1. Core Architectural Principles

- Simulation > Rendering
- Deterministic logic
- Clear module boundaries
- No hidden global state
- Small incremental features
- Data ownership is explicit

## 2. High-Level System Overview

RTS = interacting simulations.

Core Systems (Planned)
- Game Loop
- Map System (grid-based)
- Unit System
- Command System
- Pathfinding (A*)
- Resource System
- Building System
- Combat System
- AI System
- Rendering Layer

Each system must have:
- Clear responsibility
- Clear API
- No circular dependencies

## 3. World Model

### 3.1 Grid-Based World

Inspired by classic RTS like:
- Age of Empires II
- Warcraft III

Design Decision
- World = 2D tile array
- Tiles are logical units
- Units occupy tile coordinates
- Rendering is smooth but logic is discrete

### 3.2 Coordinate Systems

Tile Coordinates
```c
int tx;
int ty;
```

World Coordinates
```c
float wx;
float wy; // pixels
```

Mandatory conversion functions:
- Tile → World
- World → Tile

No direct mixing allowed.

## 4. Core Data Structures (Conceptual)

### 4.1 GameState

Owns entire simulation state:
- Map
- Units
- Players
- Global time
- Random seed (for determinism)

GameState is passed explicitly.
No hidden global state.

### 4.2 Map

Responsibilities:
- Store tile grid
- Terrain type
- Walkability
- Occupancy
- Provide queries

Does NOT:
- Move units
- Handle input
- Contain AI logic

### 4.3 Unit

Minimal initial properties:
- ID
- Tile position
- World position
- Movement state
- Speed
- Owner

Future:
- Health
- Command queue
- Attack stats
- Animation state

## 5. Game Loop Model

Basic loop:
```c
process_input();
update_simulation(dt);
render();
```

Important future decision:
- Fixed timestep vs variable timestep

Long-term goal:
- Deterministic update loop

## 6. Development Phases

Strict incremental progression.

### Phase 1 – Minimal Movement

Goal:
- Window
- Grid rendering
- Single unit
- Click tile → move one tile
- Smooth interpolation

No:
- Pathfinding
- Multiple units
- AI

Purpose:
- Establish architecture

### Phase 2 – Multi-Tile Movement

- Add movement queue
- Simple path (straight line only)
- Tile blocking

Purpose:
- Introduce command abstraction

### Phase 3 – A* Pathfinding

- Grid graph
- Open/closed sets
- Heuristic
- Path reconstruction

Learning:
- Graphs
- Algorithms
- Memory management

### Phase 4 – Multiple Units

- Unit array management
- Selection box
- Command broadcasting

Learning:
- Data ownership
- Iteration patterns

### Phase 5 – Buildings

- Tile occupancy
- Multi-tile footprint
- Build validation

Learning:
- Spatial constraints

### Phase 6 – Resources

- Resource nodes
- Gather command
- Carry capacity
- Drop-off buildings

Learning:
- State machines

### Phase 7 – Combat

- Attack range
- Cooldowns
- Health
- Death handling

Learning:
- Event-driven logic inside simulation

### Phase 8 – AI

- Finite state machines
- Behavior trees (optional later)

Learning:
- Decision systems

## 7. Future Advanced Systems

Not early, but roadmap:
- Deterministic multiplayer lockstep
- Spatial partitioning (quadtrees)
- Data-oriented design
- ECS refactor experiment
- Save/Load serialization
- Replay system

## 8. Architecture Rules

- No module should know the internal structure of another module
- Communication via functions only
- No circular includes
- No “quick hacks in main.c”

Every feature must answer:
- Which system owns this?
- Who modifies it?
- Who reads it?

## 9. Folder Structure

src/
    main.c
    core/  # defines what the world is
    game/  # defines how the world runs
    input/ # defines how the world receives commands
    render/ # defines how the world looks

docs/
    architecture.md

Will expand gradually.

## 10. Learning Objectives Mapping

System                Teaches
Grid Map              Data structures
A*                    Graph algorithms
Command Queue         Abstraction design
Unit Array            Memory layout
Combat                State machines
AI                    Behavioral modeling
Multiplayer           Deterministic simulation

## 11. Design Constraints (Intentional)

- Grid-based forever (core simulation)
- No physics engine
- No external dependencies except raylib
- C only
- No premature ECS

## 12. Long-Term Vision

Small but complete RTS simulation:
- Economy
- Combat
- AI opponent
- Replay capability

Not visually impressive.
Architecturally clean.
