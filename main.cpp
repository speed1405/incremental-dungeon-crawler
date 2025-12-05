#include "game.h"
#include <iostream>
#include <fstream>

int main() {
    GameState game;
    
    // Try to load saved game
    std::ifstream checkFile("save_game.json");
    if (checkFile.good()) {
        checkFile.close();
        std::cout << "Found saved game. Load it? (y/n): ";
        std::string response;
        std::getline(std::cin, response);
        
        if (response == "y" || response == "Y") {
            if (game.loadGame()) {
                std::cout << "Game loaded successfully!\n";
            } else {
                std::cout << "Failed to load game. Starting new game...\n";
            }
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }
    }
    
    while (game.gameRunning) {
        std::string choice = mainMenu(game);
        
        if (choice == "1") {
            // Enter dungeon
            Biome selectedBiome;
            DungeonSize selectedSize;
            
            if (dungeonSelectionMenu(game, selectedBiome, selectedSize)) {
                game.startDungeon(selectedBiome, selectedSize);
                combatMenu(game);
            }
        } else if (choice == "2") {
            // Upgrade stats
            upgradeMenu(game);
        } else if (choice == "3") {
            // View statistics
            statisticsMenu(game);
        } else if (choice == "4") {
            // Save game
            if (game.saveGame()) {
                std::cout << "\n💾 Game saved successfully!\n";
            } else {
                std::cout << "\n❌ Failed to save game!\n";
            }
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        } else if (choice == "5") {
            // Load game
            if (game.loadGame()) {
                std::cout << "\n💾 Game loaded successfully!\n";
            } else {
                std::cout << "\n❌ No save file found or failed to load!\n";
            }
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        } else if (choice == "6") {
            // Exit
            std::cout << "\n👋 Thanks for playing!\n";
            game.gameRunning = false;
        }
    }
    
    return 0;
}
