# Building Executable Guide

This guide explains how to create standalone executable files for the Incremental Dungeon Crawler game.

## Overview

Using PyInstaller, you can create executable files that:
- Run without requiring Python to be installed
- Are self-contained (include all dependencies)
- Can be distributed to other users
- Are approximately 7-10 MB in size

## Quick Start

### Windows
```bash
build-exe.bat
```
Creates `dist\DungeonCrawler.exe`

### Linux/macOS
```bash
./build-exe.sh
```
Creates `dist/DungeonCrawler`

## Prerequisites

### Python Version
- Python 3.6 or higher required
- Python 3.8+ recommended for best compatibility

### PyInstaller
The build scripts will automatically install PyInstaller if needed, but you can install it manually:

```bash
# Windows
pip install pyinstaller

# Linux/macOS
pip3 install pyinstaller
```

## Detailed Build Instructions

### Windows

1. **Open Command Prompt or PowerShell**

2. **Navigate to the game directory**
   ```bash
   cd path\to\incremental-dungeon-crawler
   ```

3. **Run the build script**
   ```bash
   build-exe.bat
   ```

4. **Find your executable**
   - Location: `dist\DungeonCrawler.exe`
   - Size: ~7-10 MB
   - Can be run on any Windows 10/11 PC

### Linux

1. **Open Terminal**

2. **Navigate to the game directory**
   ```bash
   cd /path/to/incremental-dungeon-crawler
   ```

3. **Make build script executable (first time only)**
   ```bash
   chmod +x build-exe.sh
   ```

4. **Run the build script**
   ```bash
   ./build-exe.sh
   ```

5. **Find your executable**
   - Location: `dist/DungeonCrawler`
   - Size: ~7-10 MB
   - Can be run on similar Linux distributions

### macOS

Same as Linux instructions above. The executable will work on macOS systems with the same or newer version.

## Manual Build

If you prefer to build manually or customize the build:

```bash
# Basic build
pyinstaller --onefile --name "DungeonCrawler" game.py

# With optimization
pyinstaller --onefile --name "DungeonCrawler" --optimize=2 game.py

# Clean previous builds first
pyinstaller --clean --onefile --name "DungeonCrawler" game.py
```

### Advanced Options

```bash
# Add custom icon (Windows)
pyinstaller --onefile --icon=icon.ico --name "DungeonCrawler" game.py

# Specify output directory
pyinstaller --onefile --distpath=./output --name "DungeonCrawler" game.py

# Add version information (Windows)
pyinstaller --onefile --name "DungeonCrawler" --version-file=version.txt game.py

# Enable UPX compression (if UPX is installed)
pyinstaller --onefile --upx-dir=/path/to/upx --name "DungeonCrawler" game.py
```

## Build Output

After a successful build, you'll find:

```
dist/
  └── DungeonCrawler(.exe)    # The standalone executable

build/
  └── DungeonCrawler/         # Build artifacts (can be deleted)

DungeonCrawler.spec           # PyInstaller specification file
```

### Cleaning Build Artifacts

To remove build files:

```bash
# Windows
rmdir /s /q build dist
del DungeonCrawler.spec

# Linux/macOS
rm -rf build dist DungeonCrawler.spec
```

Or use PyInstaller's clean option:
```bash
pyinstaller --clean DungeonCrawler.spec
```

## Distribution

### What to Distribute

**Only distribute the executable:**
- Windows: `dist/DungeonCrawler.exe`
- Linux/macOS: `dist/DungeonCrawler`

**Do NOT distribute:**
- `build/` directory
- `.spec` file
- Python source files (unless you want to)

### File Permissions (Linux/macOS)

Make sure the executable has execute permissions:
```bash
chmod +x DungeonCrawler
```

### Antivirus Warnings

Some antivirus software may flag PyInstaller executables as suspicious because:
- The executable contains Python interpreter
- It's not digitally signed
- It's not widely recognized

This is a **false positive**. To resolve:
1. Add the executable to your antivirus whitelist
2. Digitally sign the executable (Windows)
3. Build from source and sign yourself

## Platform-Specific Notes

### Windows

- **Compatible with:** Windows 10, Windows 11
- **May work on:** Windows 7, Windows 8.1 (not guaranteed)
- **Architecture:** 64-bit (default)
- **Size:** Typically 7-8 MB
- **Note:** Windows Defender may scan the file first time

### Linux

- **Build platform = target platform:** Build on Ubuntu 20.04 for Ubuntu 20.04+
- **Dependencies:** Executable includes Python but may need system libraries
- **Architecture:** Typically 64-bit
- **Size:** Typically 7-8 MB
- **Note:** May not work across very different distros

### macOS

- **Code signing:** May need to sign for distribution
- **Gatekeeper:** Users may need to allow app in Security settings
- **Architecture:** Intel or Apple Silicon (build on target architecture)
- **Size:** Typically 8-10 MB

## Troubleshooting

### Build Fails

**Error: "PyInstaller not found"**
```bash
pip install pyinstaller
```

**Error: "Permission denied"**
- Linux/macOS: Use `chmod +x build-exe.sh`
- Windows: Run as Administrator

**Error: "Module not found"**
- Make sure you're in the correct directory
- Check that `game.py` exists

### Executable Won't Run

**Windows: "DLL load failed"**
- Run on a different PC
- Install Visual C++ Redistributables

**Linux: "Error loading Python lib"**
- Build on the target distribution
- Check file permissions with `ls -l`

**macOS: "App is damaged"**
- Right-click → Open (instead of double-clicking)
- Or: `xattr -d com.apple.quarantine DungeonCrawler`

### Large File Size

The executable is 7-10 MB because it includes:
- Python interpreter (~5 MB)
- Standard library modules
- Game code

To reduce size:
- Use UPX compression (risky, may cause issues)
- Exclude unused modules (advanced)
- Accept the size (recommended)

## Verifying the Build

Test your executable:

```bash
# Windows
dist\DungeonCrawler.exe

# Linux/macOS
./dist/DungeonCrawler
```

The game should start and display the main menu.

## Cross-Platform Building

**Important:** PyInstaller creates executables for the **current platform only**.

To create Windows .exe from Linux:
- Not directly supported by PyInstaller
- Options:
  1. Use a Windows VM
  2. Use Wine + PyInstaller (experimental)
  3. Use a Windows build server/CI

To create Linux binary from Windows:
- Not supported
- Use WSL (Windows Subsystem for Linux)
- Or use a Linux VM/build server

## CI/CD Integration

Example GitHub Actions workflow:

```yaml
name: Build Executables

on: [push]

jobs:
  build-windows:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v2
      - uses: actions/setup-python@v2
      - run: pip install pyinstaller
      - run: pyinstaller --onefile --name DungeonCrawler game.py
      - uses: actions/upload-artifact@v2
        with:
          name: DungeonCrawler-Windows
          path: dist/DungeonCrawler.exe

  build-linux:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      - uses: actions/setup-python@v2
      - run: pip install pyinstaller
      - run: pyinstaller --onefile --name DungeonCrawler game.py
      - uses: actions/upload-artifact@v2
        with:
          name: DungeonCrawler-Linux
          path: dist/DungeonCrawler
```

## Best Practices

1. **Test the executable** on a clean machine without Python
2. **Document the target OS version** (e.g., "Windows 10+")
3. **Include README** with the executable for users
4. **Keep source separate** from the distribution
5. **Version your builds** (e.g., DungeonCrawler-v1.0.exe)

## Support

If you encounter issues:
1. Check PyInstaller documentation
2. Ensure Python version compatibility
3. Try building on the target platform
4. Check PyInstaller GitHub issues

## License

The executable contains the same code as the Python version and falls under the same license as the project.
