#!/bin/bash

# Build script for Incremental Dungeon Crawler

echo "Building Incremental Dungeon Crawler..."
echo "========================================"

# Clean previous build
if [ -f "dungeon_crawler" ]; then
    echo "Cleaning previous build..."
    make clean
fi

# Build the game
echo "Compiling..."
make

# Check if build was successful
if [ $? -eq 0 ]; then
    echo ""
    echo "✅ Build successful!"
    echo ""
    echo "Executable: dungeon_crawler"
    echo "Size: $(ls -lh dungeon_crawler | awk '{print $5}')"
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
