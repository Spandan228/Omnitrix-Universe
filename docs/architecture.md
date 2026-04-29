# Omnitrix Universe - Architecture Overview

## 1. Game Loop & Core Engine (`Game.cpp`, `Game.h`)
The `Game` class orchestrates the fundamental game loop structure:
- **Initialization:** Context creation, window management, and asset loading.
- **Input Processing (`processEvents`):** Handles OS-level events (window closing, keyboard presses, etc.).
- **State Update (`update`):** Manages time-stepping, state transitions (Menu, Playing, Game Over), and updating all active entities.
- **Rendering (`render`):** Clears the back-buffer, draws all active graphical components (UI, Game World, Particles), and displays the frame.
- **Delta Time:** Time elapsed between frames ensures consistent movement and simulation speeds independent of framerate.

## 2. Entity System & Actors
Actors in the game world inherit common traits but are functionally distinct:
- **`Alien` (Player Controller):** Represents the player character. Manages forms, transformations via the Omnitrix, user-driven movement, and basic collision/interaction handling.
- **`Enemy`:** Represents AI-controlled actors. Contains basic steering/pathfinding logic towards the player, health tracking, and states (Idle, Chase, Attack).

## 3. The Omnitrix Core (`Omnitrix.cpp`, `Omnitrix.h`)
A central gameplay mechanic that manages the transformation lifecycle:
- Tracks current selected form, cooldown state, and active transformation duration.
- Dispatches form-change requests to the player entity.

## 4. Visuals & Rendering Pipeline
The game utilizes SFML's rendering capabilities through layered drawing:
1. **Background Elements:** Base map and static environment.
2. **Entities:** Enemies and the Player, sorted dynamically if needed to simulate depth (Y-sorting).
3. **`Particle` System:** Handles transient visual effects (explosions, transformation flashes, hit sparks) ensuring they are updated independently and cleaned up upon expiration.
4. **`UI` Layer:** Renders on top of everything. The UI class reads state from the Game and Omnitrix to display the HUD (health, active alien, cooldown ring).

## 5. Directory Structure
- `assets/`: Categorized media files (textures, fonts, shaders, audio).
- `include/omnitrix/`: Public header files outlining component interfaces.
- `src/`: Implementation files corresponding to headers.
- `bin/`: Output directory for the compiled executable.
- `build/`: Temporary object files.
