# Incremental Dungeon Crawler - Feature List

## Implemented Features

### ✅ Core Gameplay
- [x] Turn-based combat system
- [x] Character progression (levels, stats, experience)
- [x] Gold economy with upgrades
- [x] Auto-battle mode for faster gameplay
- [x] Boss encounters on final floors

### ✅ Biomes (5 Total)
Each biome has unique enemy types and thematic content:
- [x] Forest (Goblin, Wolf, Bear, Troll)
- [x] Cave (Bat, Spider, Slime, Golem)
- [x] Desert (Scorpion, Snake, Mummy, Sand Elemental)
- [x] Ice Cavern (Ice Sprite, Frost Wolf, Yeti, Ice Dragon)
- [x] Volcano (Fire Imp, Lava Golem, Magma Worm, Phoenix)

### ✅ Dungeon Sizes (4 Total)
Each size has different floor counts and difficulty multipliers:
- [x] Small (5 floors, 1.0x difficulty)
- [x] Medium (10 floors, 1.5x difficulty)
- [x] Large (20 floors, 2.0x difficulty)
- [x] Epic (50 floors, 3.0x difficulty)

### ✅ Combat Mechanics
- [x] Attack/Defense stat calculations
- [x] Damage reduction based on defense
- [x] Minimum damage guarantee (1 damage)
- [x] Progressive enemy scaling by floor
- [x] Boss enemies with enhanced stats and rewards
- [x] 30% healing between floors

### ✅ Progression Systems
- [x] Experience-based leveling
- [x] Automatic stat gains on level up (+20 HP, +5 ATK, +2 DEF)
- [x] Escalating experience requirements (1.5x per level)
- [x] Gold-based permanent upgrades
- [x] Escalating upgrade costs (1.5x multiplier)

### ✅ User Interface
- [x] Clear, formatted text-based UI
- [x] Menu navigation system
- [x] Real-time combat feedback
- [x] Player and enemy stat displays
- [x] Progress indicators (floor counts, etc.)
- [x] Unicode emoji support for visual appeal

### ✅ Persistence
- [x] Save game to JSON file
- [x] Load game from JSON file
- [x] Auto-detect saved game on launch
- [x] Save all player progress and stats

### ✅ Statistics Tracking
- [x] Total floors cleared
- [x] Total dungeons completed
- [x] Current level
- [x] Gold earned
- [x] Experience points

### ✅ Quality of Life Features
- [x] Auto-battle toggle for grinding
- [x] Flee option to exit dungeons
- [x] Full heal on death (return to town)
- [x] Full heal when starting new dungeon
- [x] Upgrade preview showing costs

### ✅ Build System
- [x] Makefile for easy compilation
- [x] Cross-platform support (Linux, macOS, Windows)
- [x] Single executable output
- [x] Clean build targets
- [x] Build and test scripts

## Technical Details

### Programming Language
- **C++17** with STL

### Dependencies
- Standard C++ Library only
- No external dependencies required

### Build Requirements
- C++17 compatible compiler (g++, clang++, MSVC)
- Make (optional, can compile manually)

### File Structure
```
incremental-dungeon-crawler/
├── game.h              # Game classes and declarations
├── game.cpp            # Game logic implementation
├── main.cpp            # Entry point and main game loop
├── Makefile            # Build configuration
├── build.sh            # Build script
├── test.sh             # Automated test script
├── README.md           # Documentation
└── .gitignore          # Git ignore rules
```

### Executable Size
- Approximately 88-90 KB (optimized build)
- Single self-contained executable
- No runtime dependencies

## Game Balance

### Starting Stats
- Health: 100
- Attack: 10
- Defense: 5
- Gold: 0
- Level: 1

### Level Up Bonuses
- +20 Max Health (full heal)
- +5 Attack
- +2 Defense
- Experience requirement: 1.5x previous

### Upgrade Costs (Exponential)
- Health: 50 * (1.5^upgrades)
- Attack: 100 * (1.5^upgrades)
- Defense: 80 * (1.5^upgrades)

### Enemy Scaling
- Base stats increase by 20% per floor
- Multiplied by dungeon difficulty
- Boss enemies: 2.5x health, 1.5x attack/defense, 3x rewards

### Rewards
- Gold: 10 * floor_multiplier * difficulty
- Experience: 20 * floor_multiplier * difficulty
- Boss multiplier: 3x for both

## Future Enhancement Ideas
(Not implemented, but possible extensions)
- Equipment system
- Skills and abilities
- Prestige/rebirth mechanics
- Multiple save slots
- Leaderboards
- Achievements system
- Consumable items
- Rare/elite enemies
- Random events
- Party system
