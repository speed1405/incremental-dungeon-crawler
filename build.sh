#!/bin/bash

# Build script for Incremental Dungeon Crawler

echo "Building Incremental Dungeon Crawler..."
echo "========================================"

# Parse command line arguments
BUILD_TYPE="static"
if [[ "$1" == "dynamic" ]]; then
    BUILD_TYPE="dynamic"
fi

# Clean previous build
if [[ -f "dungeon_crawler" ]]; then
    echo "Cleaning previous build..."
    make clean
fi

# Build the game
if [[ "$BUILD_TYPE" == "static" ]]; then
    echo "Compiling (static build - standalone executable)..."
    make static
else
    echo "Compiling (dynamic build)..."
    make
fi

# Check if build was successful
if [[ $? -eq 0 ]]; then
    echo ""
    echo "✅ Build successful!"
    echo ""
    echo "Executable: dungeon_crawler"
    echo "Size: $(ls -lh dungeon_crawler | awk '{print $5}')"
    if [[ "$BUILD_TYPE" == "static" ]]; then
        echo "Type: Statically linked (standalone, no dependencies)"
    else
        echo "Type: Dynamically linked"
    fi
    echo ""
    echo "To run the game:"
    echo "  ./dungeon_crawler"
    echo ""
    echo "Or use:"
    echo "  make run"
else
    echo ""
    echo "❌ Build failed!"
    exit 1
fi
