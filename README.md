# Incremental Dungeon Crawler

A text-based incremental dungeon crawler game with multiple biomes and dungeon sizes, available in both **Python** and **C++**.

## Features

### 🌍 Multiple Biomes
- **Forest** - Battle goblins, wolves, bears, and trolls
- **Cave** - Fight bats, spiders, slimes, and golems
- **Desert** - Face scorpions, snakes, mummies, and sand elementals
- **Ice Cavern** - Combat ice sprites, frost wolves, yetis, and ice dragons
- **Volcano** - Challenge fire imps, lava golems, magma worms, and phoenixes

### 📏 Dungeon Sizes
- **Small** - 5 floors, 1.0x difficulty
- **Medium** - 10 floors, 1.5x difficulty
- **Large** - 20 floors, 2.0x difficulty
- **Epic** - 50 floors, 3.0x difficulty

### 🎮 Game Mechanics
- **Progressive Combat** - Turn-based combat with scaling enemy difficulty
- **Character Progression** - Level up and gain stats through experience
- **Incremental Upgrades** - Spend gold to permanently upgrade health, attack, and defense
- **Auto-Battle Mode** - Toggle automatic combat for faster progression
- **Boss Fights** - Face powerful bosses on final floors with greater rewards
- **Save/Load System** - Save your progress and continue later

### 🎯 Core Gameplay Loop
1. Choose a biome and dungeon size
2. Battle through floors of increasingly difficult enemies
3. Defeat enemies to earn gold and experience
4. Level up to gain stats automatically
5. Spend gold on permanent upgrades
6. Complete dungeons to earn achievements
7. Challenge harder dungeons with your upgraded character

## Building and Running the Game

The game is available in two versions:
1. **Python version** (Recommended for quick start)
2. **C++ version** (Compiled for maximum performance)

### Python Version

#### Requirements
- Python 3.6 or higher
- No external dependencies (uses only standard library)

#### Running the Game
```bash
python3 game.py
```

Or make it executable and run directly:
```bash
chmod +x game.py
./game.py
```

The Python version requires no compilation and works on any platform with Python installed.

#### Building Standalone Executable (Optional)

You can create a standalone executable (.exe on Windows, binary on Linux/macOS) that doesn't require Python to be installed:

**Windows:**
```bash
build-exe.bat
```
This creates `dist/DungeonCrawler.exe` which can be distributed and run on any Windows PC.

**Linux/macOS:**
```bash
./build-exe.sh
```
This creates `dist/DungeonCrawler` which can be distributed and run on the same platform.

**Manual Build:**
```bash
# Install PyInstaller (one time only)
pip install pyinstaller

# Build executable
pyinstaller --onefile --name "DungeonCrawler" game.py

# The executable will be in the dist/ directory
```

The executable is self-contained (~7-10 MB) and includes Python and all dependencies.

### C++ Version

#### Requirements
- C++17 compatible compiler (g++, clang++, MSVC)
- Make (optional, for using Makefile)
- MinGW-w64 (for cross-compiling Windows executables on Linux/macOS)

#### Compilation

The C++ game can be compiled as a **standalone executable** (static build) or with dynamic linking. Static builds are larger but contain all dependencies and can run anywhere without external libraries.

#### Quick Build (Standalone Executable - Recommended):

**Linux/macOS:**
```bash
./build.sh
```
This creates a standalone executable with all dependencies included.

**Windows (cross-compile from Linux/macOS):**
```bash
./build-windows.sh
```
This creates a Windows executable (dungeon_crawler.exe) using MinGW cross-compiler.

#### Using Make (Linux/macOS):

**Static build (standalone executable):**
```bash
make static
```

**Dynamic build:**
```bash
make
```

#### Using Make for Windows (cross-compile from Linux/macOS):

**Static Windows build (recommended):**
```bash
make windows-static
```

**Dynamic Windows build:**
```bash
make windows
```

**Clean Windows build files:**
```bash
make clean-windows
```

#### Using Make on Windows (with MinGW):
```bash
mingw32-make static
```

#### Manual Compilation (any platform):

**Standalone executable (static linking):**
```bash
g++ -std=c++17 -Wall -Wextra -O2 -o dungeon_crawler main.cpp game.cpp -static -static-libgcc -static-libstdc++
```

**Dynamic linking:**
```bash
g++ -std=c++17 -Wall -Wextra -O2 -o dungeon_crawler main.cpp game.cpp
```

**Windows cross-compilation (Linux/macOS):**
```bash
x86_64-w64-mingw32-g++ -std=c++17 -Wall -Wextra -O2 -o dungeon_crawler.exe main.cpp game.cpp -static -static-libgcc -static-libstdc++
```

#### On Windows with MSVC:
```bash
cl /EHsc /std:c++17 /Fe:dungeon_crawler.exe main.cpp game.cpp
```

**Note:** Static builds are larger (~2.4MB) but are completely standalone and portable. Dynamic builds are smaller (~88KB) but require system libraries to be present.

## Running the Game

### Python Version:
```bash
python3 game.py
```

### C++ Version:

#### Linux/macOS:
```bash
./dungeon_crawler
```

#### Windows:
```bash
dungeon_crawler.exe
```

Or using Make (C++ version only):
```bash
make run
```

## How to Play

### Main Menu
1. **Enter Dungeon** - Start a new dungeon run
2. **Upgrade Stats** - Spend gold on permanent stat increases
3. **View Statistics** - Check your achievements and progress
4. **Save Game** - Save your current progress
5. **Load Game** - Load a previously saved game
6. **Exit** - Quit the game

### Combat
- **Attack** - Deal damage to the enemy
- **Auto Battle** - Toggle automatic combat mode
- **Flee** - Return to town (lose progress in current dungeon)

### Tips
- Start with Small dungeons to build up gold and levels
- Upgrade your stats regularly to handle harder content
- Each floor cleared heals you for 30% of max health
- Boss enemies appear on the final floor of each dungeon
- Use Auto Battle mode to progress faster once you're strong enough

## Game Progression

### Early Game
- Focus on completing Small Forest dungeons
- Upgrade your health and attack early
- Build up to level 5-10 before attempting Medium dungeons

### Mid Game
- Explore different biomes for variety
- Attempt Medium and Large dungeons
- Balance health, attack, and defense upgrades

### Late Game
- Challenge Epic dungeons for maximum rewards
- Push into harder biomes like Ice and Volcano
- Optimize your build for efficient farming

## Save System

The game automatically saves to `save_game.json` when you select "Save Game" from the main menu. Your save includes:
- Character stats and progress
- Gold and experience
- Total floors cleared and dungeons completed

## Clean Build

To remove compiled files:
```bash
make clean
```

## License

This project is open source and available for educational and entertainment purposes.