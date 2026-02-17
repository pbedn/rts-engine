# Raylib + C + Sublime + Clangd Setup (Linux / Ubuntu)

Context: RTS learning project focused on architecture, determinism, and explicit ownership.

This setup provides:

- Raylib built locally (no global dependency)
- Static linking
- Makefile as build authority
- CMake only for compile_commands.json
- Sublime Text as editor
- clangd for LSP/intellisense
- F5 → build and run

---

# 1. Install Required System Packages

## 1.1 Build Essentials

```bash
sudo apt update
sudo apt install build-essential cmake git
```

## 1.2 Raylib Dependencies (Linux Desktop)

Even if you use Wayland, X11 development headers are required by default:

```bash
sudo apt install \
    libx11-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev \
    libgl1-mesa-dev
```

## 1.3 Install clangd

```bash
sudo apt install clangd
```

---

# 2. Build Raylib From Source (Local Copy)

Clone raylib somewhere outside your RTS project:

```bash
git clone https://github.com/raysan5/raylib.git
cd raylib
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

After build completes, locate:

```
libraylib.a
raylib.h
```

---

# 3. RTS Project Structure

Example structure:

```
rts-engine/
│
├── src/
│   └── main.c
│
├── include/
│   └── raylib.h
│
├── lib/
│   └── libraylib.a
│
├── build/
│
├── build_clang/
│
├── Makefile
├── CMakeLists.txt
└── compile_commands.json (symlink)
```

Copy raylib artifacts into your project:

```bash
cp ~/dev/raylib/build/raylib/libraylib.a lib/
cp ~/dev/raylib/src/raylib.h include/
```

Now raylib is owned by your project.

---

# 4. Minimal main.c

Create `src/main.c`:

```c
#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "RTS Engine");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Phase 1", 350, 200, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
```

---

# 5. Makefile (Build Authority)

Create `Makefile` in project root:

```make
CC = gcc

CFLAGS = -Iinclude -Wall -Wextra -std=c11
LDFLAGS = -Llib -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

SRC = src/main.c
OUT = build/rts

all:
	$(CC) $(SRC) $(CFLAGS) $(LDFLAGS) -o $(OUT)

clean:
	rm -f $(OUT)
```

Build and run:

```bash
make
./build/rts
```

Key concepts:

- `-Iinclude` → header search path
- `-Llib` → library search path
- `-lraylib` → links `libraylib.a`
- system libraries explicitly listed

Makefile is the single source of truth for building.

---

# 6. Generate compile_commands.json (For clangd)

We use CMake only to generate compilation database.

## 6.1 Create CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.16)
project(rts C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

add_executable(rts
    src/main.c
)

target_link_libraries(rts
    ${CMAKE_SOURCE_DIR}/lib/libraylib.a
    m dl pthread GL rt X11
)
```

## 6.2 Generate Database

```bash
mkdir -p build_clang
cd build_clang
cmake ..
```

This creates:

```
build_clang/compile_commands.json
```

## 6.3 Symlink to Root

```bash
ln -sf build_clang/compile_commands.json ../compile_commands.json
```

Now clangd will detect it automatically.

---

# 7. Sublime Text Configuration

## 7.1 Install Packages

Install via Package Control:

- LSP
- LSP-clangd

## 7.2 Optional clangd Arguments

In LSP settings:

```json
{
    "clangd.arguments": [
        "--compile-commands-dir=${project_path}"
    ]
}
```

Restart Sublime.

You now get:

- Go to definition
- Header resolution
- Diagnostics
- Autocomplete

---

# 8. F5 → Build and Run

## 8.1 Create Build System

Tools → Build System → New Build System

Paste:

```json
{
    "working_dir": "${project_path}",
    "shell_cmd": "make && ./build/rts",
    "selector": "source.c",
    "variants":
    [
        {
            "name": "Build Only",
            "shell_cmd": "make"
        }
    ]
}
```

Save as:

```
rts.sublime-build
```

Select:

```
Tools → Build System → rts
```

## 8.2 Bind F5

Preferences → Key Bindings

Add:

```json
[
    { "keys": ["f5"], "command": "build" }
]
```

Now:

- F5 → build and run
- Ctrl+B → same
- Ctrl+Shift+B → build only

---

# 9. Clean Responsibility Separation

| Tool        | Responsibility |
|------------|---------------|
| GCC        | Compilation   |
| Makefile   | Build logic   |
| Raylib     | Rendering     |
| CMake      | Compile DB only |
| clangd     | Code analysis |
| Sublime    | Editor UI     |

No global installs.
No hidden IDE magic.
Explicit linking.
Explicit ownership.

Architecture first. Rendering second.
