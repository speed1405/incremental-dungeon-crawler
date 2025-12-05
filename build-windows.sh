#!/bin/bash

# Build script for Windows executable using MinGW cross-compiler

echo "Building Incremental Dungeon Crawler for Windows..."
echo "===================================================="

# Parse command line arguments
BUILD_TYPE="static"
if [[ "$1" == "dynamic" ]]; then
    BUILD_TYPE="dynamic"
fi

# Check if MinGW is installed
if ! command -v x86_64-w64-mingw32-g++ &> /dev/null; then
    echo "❌ Error: MinGW-w64 cross-compiler not found!"
    echo ""
    echo "Please install it using:"
    echo "  Ubuntu/Debian: sudo apt-get install mingw-w64"
    echo "  Fedora/RHEL:   sudo dnf install mingw64-gcc-c++"
    echo "  Arch Linux:    sudo pacman -S mingw-w64-gcc"
    exit 1
fi

# Clean previous Windows build
if [[ -f "dungeon_crawler.exe" ]]; then
    echo "Cleaning previous Windows build..."
    make clean-windows
fi

# Build the game for Windows
if [[ "$BUILD_TYPE" == "static" ]]; then
    echo "Cross-compiling for Windows (static build - standalone executable)..."
    make windows-static
else
    echo "Cross-compiling for Windows (dynamic build)..."
    make windows
fi

# Check if build was successful
if [[ $? -eq 0 ]]; then
    echo ""
    echo "✅ Windows build successful!"
    echo ""
    echo "Executable: dungeon_crawler.exe"
    if [[ -f "dungeon_crawler.exe" ]]; then
        echo "Size: $(ls -lh dungeon_crawler.exe | awk '{print $5}')"
    fi
    if [[ "$BUILD_TYPE" == "static" ]]; then
        echo "Type: Statically linked (standalone, no dependencies)"
    else
        echo "Type: Dynamically linked (requires MinGW runtime DLLs)"
    fi
    echo ""
    echo "The executable can be run on Windows systems."
    echo "Transfer dungeon_crawler.exe to a Windows machine to play!"
else
    echo ""
    echo "❌ Build failed!"
    exit 1
fi
