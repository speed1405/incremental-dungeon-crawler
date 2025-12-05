#!/bin/bash
# Build script for creating executable from Python game
# This creates a standalone executable for the current platform

echo "=========================================="
echo "  Building Dungeon Crawler Executable"
echo "=========================================="
echo ""

# Check if Python 3 is installed
if ! command -v python3 &> /dev/null; then
    echo "❌ Error: Python 3 is not installed"
    exit 1
fi

echo "✅ Python 3 found"

# Check if PyInstaller is installed
if ! python3 -m pip show pyinstaller &> /dev/null; then
    echo "⚠️  PyInstaller not found. Installing..."
    python3 -m pip install pyinstaller
    if [ $? -ne 0 ]; then
        echo "❌ Failed to install PyInstaller"
        exit 1
    fi
    echo "✅ PyInstaller installed"
else
    echo "✅ PyInstaller found"
fi

echo ""
echo "🔨 Building executable..."
echo ""

# Build the executable
python3 -m PyInstaller --onefile --name "DungeonCrawler" game.py

if [ $? -eq 0 ]; then
    echo ""
    echo "=========================================="
    echo "  ✅ Build Successful!"
    echo "=========================================="
    echo ""
    
    # Check if dist directory exists and has the executable
    if [ -f "dist/DungeonCrawler" ] || [ -f "dist/DungeonCrawler.exe" ]; then
        echo "Executable location: dist/DungeonCrawler"
        ls -lh dist/DungeonCrawler* 2>/dev/null || true
        echo ""
        echo "To run the game:"
        echo "  ./dist/DungeonCrawler"
        echo ""
        echo "To distribute, copy the file from the dist/ directory"
    fi
else
    echo ""
    echo "❌ Build failed!"
    exit 1
fi
