#!/bin/bash
# Python build/run script for Incremental Dungeon Crawler

echo "=========================================="
echo "  Incremental Dungeon Crawler - Python"
echo "=========================================="
echo ""

# Check if Python 3 is installed
if ! command -v python3 &> /dev/null; then
    echo "❌ Error: Python 3 is not installed"
    echo "Please install Python 3.6 or higher"
    exit 1
fi

# Display Python version
PYTHON_VERSION=$(python3 --version)
echo "✅ Found: $PYTHON_VERSION"
echo ""

# Check if game.py exists
if [ ! -f "game.py" ]; then
    echo "❌ Error: game.py not found"
    exit 1
fi

echo "✅ Game file found: game.py"
echo ""

# Make executable if not already
chmod +x game.py

echo "🎮 Starting game..."
echo ""

# Run the game
python3 game.py

echo ""
echo "👋 Game ended"
