# Quick Start Guide - Python Version

## 🎮 Playing the Game

### Option 1: Run with Python (Easiest)
```bash
python3 game.py
```

### Option 2: Run with Script
```bash
./run-python.sh
```

### Option 3: Run Standalone Executable
```bash
# First build the exe (one time only)
./build-exe.sh          # Linux/macOS
# or
build-exe.bat           # Windows

# Then run it
./dist/DungeonCrawler   # Linux/macOS
# or
dist\DungeonCrawler.exe # Windows
```

## 🔧 Requirements

**To run from source:**
- Python 3.6 or higher
- That's it! No other dependencies.

**To build executable:**
- Python 3.6+
- PyInstaller (auto-installed by build scripts)

## 📦 What You Get

- **game.py** - The complete game in one file
- **Build scripts** - For creating executables
- **Documentation** - Everything you need to know

## 🎯 Game Features

- 5 unique biomes to explore
- 4 dungeon sizes (5 to 50 floors)
- Turn-based combat
- Level up and upgrade system
- Auto-battle mode
- Boss fights
- Save/load your progress

## 📚 Documentation

- **README.md** - Main documentation
- **PYTHON_README.md** - Python-specific details
- **BUILD_EXE.md** - Building executables
- **PYTHON_SUMMARY.md** - Implementation summary

## ❓ Quick FAQ

**Q: Do I need to install anything?**
A: Just Python 3.6+. No other dependencies.

**Q: How do I create an .exe file?**
A: Run `build-exe.bat` on Windows or `./build-exe.sh` on Linux/macOS

**Q: Can I distribute the game?**
A: Yes! Share the .exe file or the game.py source.

**Q: Where are my saves?**
A: In `save_game.json` in the same directory as the game.

**Q: Is this the same as the C++ version?**
A: Yes! Same features, same gameplay, written in Python.

## 🚀 One-Line Quick Start

```bash
python3 game.py
```

That's it! Enjoy the game! 🏰⚔️
