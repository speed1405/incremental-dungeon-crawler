#!/bin/bash

# Automated test script for Incremental Dungeon Crawler

echo "Testing Incremental Dungeon Crawler"
echo "===================================="
echo ""

# Test 1: Check executable exists
echo "Test 1: Checking if executable exists..."
if [ -f "dungeon_crawler" ]; then
    echo "✅ Executable found"
else
    echo "❌ Executable not found. Building..."
    make
    if [ $? -ne 0 ]; then
        echo "❌ Build failed"
        exit 1
    fi
fi
echo ""

# Test 2: Check if game launches and exits properly
echo "Test 2: Testing game launch and exit..."
echo "6" | timeout 5 ./dungeon_crawler > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✅ Game launches and exits correctly"
else
    echo "❌ Game failed to launch or exit"
fi
echo ""

# Test 3: Test statistics menu
echo "Test 3: Testing statistics menu..."
echo -e "3\n6\n" | timeout 5 ./dungeon_crawler > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✅ Statistics menu works"
else
    echo "❌ Statistics menu failed"
fi
echo ""

# Test 4: Test save game
echo "Test 4: Testing save game functionality..."
echo -e "4\n6\n" | timeout 5 ./dungeon_crawler > /dev/null 2>&1
if [ -f "save_game.json" ]; then
    echo "✅ Save game created"
    rm -f save_game.json
else
    echo "❌ Save game not created"
fi
echo ""

# Test 5: Test entering dungeon and fleeing
echo "Test 5: Testing dungeon entry and flee..."
echo -e "1\n1\n1\n3\n6\n" | timeout 10 ./dungeon_crawler > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✅ Dungeon entry and flee works"
else
    echo "❌ Dungeon entry or flee failed"
fi
echo ""

# Test 6: Check file sizes
echo "Test 6: Checking executable size..."
SIZE=$(stat -c%s "dungeon_crawler" 2>/dev/null || stat -f%z "dungeon_crawler" 2>/dev/null)
if [ $SIZE -gt 0 ]; then
    echo "✅ Executable size: $SIZE bytes"
else
    echo "❌ Cannot determine executable size"
fi
echo ""

echo "===================================="
echo "Testing complete!"
echo ""
echo "Manual testing recommended:"
echo "  1. Run ./dungeon_crawler"
echo "  2. Try entering different biomes and dungeon sizes"
echo "  3. Test combat and leveling"
echo "  4. Test upgrades"
echo "  5. Test save/load functionality"
