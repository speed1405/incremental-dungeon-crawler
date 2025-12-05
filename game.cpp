#include "game.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <algorithm>
#include <thread>
#include <chrono>

// Random number generator
static std::random_device rd;
static std::mt19937 gen(rd());

// Enemy implementation
Enemy::Enemy(const std::string& n, int h, int atk, int def, int gold, int exp)
    : name(n), health(h), maxHealth(h), attack(atk), defense(def), 
      goldReward(gold), expReward(exp) {}

bool Enemy::isAlive() const {
    return health > 0;
}

int Enemy::takeDamage(int damage) {
    int actualDamage = std::max(1, damage - defense);
    health = std::max(0, health - actualDamage);
    return actualDamage;
}

// Player implementation
Player::Player()
    : name("Hero"), level(1), health(100), maxHealth(100), attack(10), 
      defense(5), gold(0), experience(0), expToNextLevel(100), 
      floorsCleared(0), dungeonsCompleted(0) {}

bool Player::isAlive() const {
    return health > 0;
}

int Player::takeDamage(int damage) {
    int actualDamage = std::max(1, damage - defense);
    health = std::max(0, health - actualDamage);
    return actualDamage;
}

void Player::heal(int amount) {
    health = std::min(maxHealth, health + amount);
}

void Player::fullHeal() {
    health = maxHealth;
}

void Player::gainExperience(int exp) {
    experience += exp;
    while (experience >= expToNextLevel) {
        levelUp();
    }
}

void Player::levelUp() {
    experience -= expToNextLevel;
    level++;
    maxHealth += 20;
    health = maxHealth;
    attack += 5;
    defense += 2;
    expToNextLevel = static_cast<int>(expToNextLevel * 1.5);
}

bool Player::canAfford(int cost) const {
    return gold >= cost;
}

bool Player::spendGold(int amount) {
    if (canAfford(amount)) {
        gold -= amount;
        return true;
    }
    return false;
}

// GameState implementation
GameState::GameState()
    : currentFloor(0), autoBattle(false), inDungeon(false), gameRunning(true) {
    initializeData();
}

void GameState::initializeData() {
    // Initialize biome names
    biomeNames[Biome::FOREST] = "Forest";
    biomeNames[Biome::CAVE] = "Cave";
    biomeNames[Biome::DESERT] = "Desert";
    biomeNames[Biome::ICE] = "Ice Cavern";
    biomeNames[Biome::VOLCANO] = "Volcano";
    
    // Initialize enemy types
    enemyTypes[Biome::FOREST] = {"Goblin", "Wolf", "Bear", "Troll"};
    enemyTypes[Biome::CAVE] = {"Bat", "Spider", "Slime", "Golem"};
    enemyTypes[Biome::DESERT] = {"Scorpion", "Snake", "Mummy", "Sand Elemental"};
    enemyTypes[Biome::ICE] = {"Ice Sprite", "Frost Wolf", "Yeti", "Ice Dragon"};
    enemyTypes[Biome::VOLCANO] = {"Fire Imp", "Lava Golem", "Magma Worm", "Phoenix"};
    
    // Initialize dungeon size info
    dungeonSizeInfo[DungeonSize::SMALL] = {"Small", 5, 1.0};
    dungeonSizeInfo[DungeonSize::MEDIUM] = {"Medium", 10, 1.5};
    dungeonSizeInfo[DungeonSize::LARGE] = {"Large", 20, 2.0};
    dungeonSizeInfo[DungeonSize::EPIC] = {"Epic", 50, 3.0};
}

Player& GameState::getPlayer() {
    return player;
}

const Player& GameState::getPlayer() const {
    return player;
}

Biome GameState::getCurrentBiome() const {
    return currentBiome;
}

DungeonSize GameState::getCurrentDungeonSize() const {
    return currentDungeonSize;
}

int GameState::getCurrentFloor() const {
    return currentFloor;
}

std::shared_ptr<Enemy> GameState::getCurrentEnemy() const {
    return currentEnemy;
}

bool GameState::isAutoBattle() const {
    return autoBattle;
}

bool GameState::isInDungeon() const {
    return inDungeon;
}

void GameState::startDungeon(Biome biome, DungeonSize size) {
    currentBiome = biome;
    currentDungeonSize = size;
    currentFloor = 1;
    inDungeon = true;
    player.fullHeal();
    spawnEnemy();
}

void GameState::spawnEnemy() {
    if (!inDungeon) return;
    
    // Base enemy stats scale with floor
    double floorMultiplier = 1.0 + (currentFloor - 1) * 0.2;
    double difficulty = dungeonSizeInfo[currentDungeonSize].difficultyMultiplier;
    
    int baseHealth = static_cast<int>(50 * floorMultiplier * difficulty);
    int baseAttack = static_cast<int>(8 * floorMultiplier * difficulty);
    int baseDefense = static_cast<int>(3 * floorMultiplier * difficulty);
    int baseGold = static_cast<int>(10 * floorMultiplier * difficulty);
    int baseExp = static_cast<int>(20 * floorMultiplier * difficulty);
    
    // Select random enemy type
    const auto& types = enemyTypes[currentBiome];
    std::uniform_int_distribution<> dis(0, types.size() - 1);
    std::string enemyName = types[dis(gen)];
    
    // Boss on final floor
    if (currentFloor == dungeonSizeInfo[currentDungeonSize].floors) {
        enemyName = biomeNames[currentBiome] + " Boss";
        baseHealth = static_cast<int>(baseHealth * 2.5);
        baseAttack = static_cast<int>(baseAttack * 1.5);
        baseDefense = static_cast<int>(baseDefense * 1.5);
        baseGold = static_cast<int>(baseGold * 3);
        baseExp = static_cast<int>(baseExp * 3);
    }
    
    currentEnemy = std::make_shared<Enemy>(enemyName, baseHealth, baseAttack, 
                                          baseDefense, baseGold, baseExp);
}

CombatResult GameState::attackEnemy() {
    CombatResult result = {0, false, 0, false, false, false};
    
    if (!currentEnemy || !currentEnemy->isAlive()) {
        return result;
    }
    
    // Player attacks
    result.playerDamage = currentEnemy->takeDamage(player.attack);
    
    // Check if enemy is defeated
    if (!currentEnemy->isAlive()) {
        result.enemyDefeated = true;
        player.gold += currentEnemy->goldReward;
        player.gainExperience(currentEnemy->expReward);
        player.floorsCleared++;
        
        // Check if dungeon is completed
        if (currentFloor >= dungeonSizeInfo[currentDungeonSize].floors) {
            result.dungeonCompleted = true;
            player.dungeonsCompleted++;
            currentFloor = 0;
            currentEnemy = nullptr;
            inDungeon = false;
            autoBattle = false;
            return result;
        } else {
            // Advance to next floor
            currentFloor++;
            result.floorCleared = true;
            player.heal(static_cast<int>(player.maxHealth * 0.3));
            spawnEnemy();
            return result;
        }
    }
    
    // Enemy attacks back
    if (currentEnemy->isAlive()) {
        result.enemyDamage = player.takeDamage(currentEnemy->attack);
        
        if (!player.isAlive()) {
            result.playerDied = true;
            // Reset to town
            currentFloor = 0;
            currentEnemy = nullptr;
            inDungeon = false;
            autoBattle = false;
            player.fullHeal();
        }
    }
    
    return result;
}

bool GameState::upgradeStat(const std::string& stat) {
    int cost = getUpgradeCost(stat);
    
    if (cost == 0 || !player.spendGold(cost)) {
        return false;
    }
    
    if (stat == "health") {
        player.maxHealth += 20;
        player.health = player.maxHealth;
    } else if (stat == "attack") {
        player.attack += 5;
    } else if (stat == "defense") {
        player.defense += 2;
    }
    
    return true;
}

int GameState::getUpgradeCost(const std::string& stat) const {
    if (stat == "health") {
        return static_cast<int>(50 * std::pow(1.5, player.maxHealth / 20 - 5));
    } else if (stat == "attack") {
        return static_cast<int>(100 * std::pow(1.5, player.attack / 5 - 2));
    } else if (stat == "defense") {
        return static_cast<int>(80 * std::pow(1.5, player.defense / 2 - 2));
    }
    return 0;
}

void GameState::toggleAutoBattle() {
    autoBattle = !autoBattle;
}

void GameState::fleeDungeon() {
    currentFloor = 0;
    currentEnemy = nullptr;
    inDungeon = false;
    autoBattle = false;
    player.fullHeal();
}

bool GameState::saveGame(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    file << "{\n";
    file << "  \"player\": {\n";
    file << "    \"name\": \"" << player.name << "\",\n";
    file << "    \"level\": " << player.level << ",\n";
    file << "    \"health\": " << player.health << ",\n";
    file << "    \"maxHealth\": " << player.maxHealth << ",\n";
    file << "    \"attack\": " << player.attack << ",\n";
    file << "    \"defense\": " << player.defense << ",\n";
    file << "    \"gold\": " << player.gold << ",\n";
    file << "    \"experience\": " << player.experience << ",\n";
    file << "    \"expToNextLevel\": " << player.expToNextLevel << ",\n";
    file << "    \"floorsCleared\": " << player.floorsCleared << ",\n";
    file << "    \"dungeonsCompleted\": " << player.dungeonsCompleted << "\n";
    file << "  },\n";
    file << "  \"currentFloor\": " << currentFloor << ",\n";
    file << "  \"autoBattle\": " << (autoBattle ? "true" : "false") << ",\n";
    file << "  \"inDungeon\": " << (inDungeon ? "true" : "false") << "\n";
    file << "}\n";
    
    file.close();
    return true;
}

bool GameState::loadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    // Simple JSON parsing (basic implementation)
    std::string line;
    while (std::getline(file, line)) {
        // Remove whitespace and quotes
        line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
        
        if (line.find("\"level\":") != std::string::npos) {
            player.level = std::stoi(line.substr(line.find(":") + 1, line.find(",") - line.find(":") - 1));
        } else if (line.find("\"health\":") != std::string::npos) {
            player.health = std::stoi(line.substr(line.find(":") + 1, line.find(",") - line.find(":") - 1));
        } else if (line.find("\"maxHealth\":") != std::string::npos) {
            player.maxHealth = std::stoi(line.substr(line.find(":") + 1, line.find(",") - line.find(":") - 1));
        } else if (line.find("\"attack\":") != std::string::npos) {
            player.attack = std::stoi(line.substr(line.find(":") + 1, line.find(",") - line.find(":") - 1));
        } else if (line.find("\"defense\":") != std::string::npos) {
            player.defense = std::stoi(line.substr(line.find(":") + 1, line.find(",") - line.find(":") - 1));
        } else if (line.find("\"gold\":") != std::string::npos) {
            player.gold = std::stoi(line.substr(line.find(":") + 1, line.find(",") - line.find(":") - 1));
        } else if (line.find("\"experience\":") != std::string::npos) {
            player.experience = std::stoi(line.substr(line.find(":") + 1, line.find(",") - line.find(":") - 1));
        } else if (line.find("\"expToNextLevel\":") != std::string::npos) {
            player.expToNextLevel = std::stoi(line.substr(line.find(":") + 1, line.find(",") - line.find(":") - 1));
        } else if (line.find("\"floorsCleared\":") != std::string::npos) {
            player.floorsCleared = std::stoi(line.substr(line.find(":") + 1, line.find(",") - line.find(":") - 1));
        } else if (line.find("\"dungeonsCompleted\":") != std::string::npos) {
            player.dungeonsCompleted = std::stoi(line.substr(line.find(":") + 1));
        }
    }
    
    file.close();
    return true;
}

std::string GameState::getBiomeName(Biome biome) const {
    return biomeNames.at(biome);
}

DungeonSizeInfo GameState::getDungeonSizeInfo(DungeonSize size) const {
    return dungeonSizeInfo.at(size);
}

std::vector<Biome> GameState::getAllBiomes() const {
    return {Biome::FOREST, Biome::CAVE, Biome::DESERT, Biome::ICE, Biome::VOLCANO};
}

std::vector<DungeonSize> GameState::getAllDungeonSizes() const {
    return {DungeonSize::SMALL, DungeonSize::MEDIUM, DungeonSize::LARGE, DungeonSize::EPIC};
}

// UI functions
void clearScreen() {
#ifdef _WIN32
    int ret = system("cls");
    (void)ret; // Suppress unused variable warning
#else
    int ret = system("clear");
    (void)ret; // Suppress unused variable warning
#endif
}

void printHeader(const std::string& text) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << "  " << text << "\n";
    std::cout << std::string(60, '=') << "\n";
}

void printPlayerStats(const Player& player) {
    std::cout << "\n📊 Player Stats:\n";
    std::cout << "  Level: " << player.level << " | HP: " << player.health 
              << "/" << player.maxHealth << "\n";
    std::cout << "  Attack: " << player.attack << " | Defense: " << player.defense << "\n";
    std::cout << "  Gold: " << player.gold << " | EXP: " << player.experience 
              << "/" << player.expToNextLevel << "\n";
    std::cout << "  Floors Cleared: " << player.floorsCleared 
              << " | Dungeons: " << player.dungeonsCompleted << "\n";
}

void printEnemyStats(const Enemy& enemy) {
    std::cout << "\n⚔️  Enemy: " << enemy.name << "\n";
    std::cout << "  HP: " << enemy.health << "/" << enemy.maxHealth << "\n";
    std::cout << "  Attack: " << enemy.attack << " | Defense: " << enemy.defense << "\n";
}

// Menu functions
std::string mainMenu(const GameState& game) {
    clearScreen();
    printHeader("🏰 INCREMENTAL DUNGEON CRAWLER 🏰");
    printPlayerStats(game.getPlayer());
    
    std::cout << "\n📜 Main Menu:\n";
    std::cout << "  1. Enter Dungeon\n";
    std::cout << "  2. Upgrade Stats\n";
    std::cout << "  3. View Statistics\n";
    std::cout << "  4. Save Game\n";
    std::cout << "  5. Load Game\n";
    std::cout << "  6. Exit\n";
    
    std::string choice;
    std::cout << "\nChoose an option: ";
    std::getline(std::cin, choice);
    return choice;
}

bool dungeonSelectionMenu(GameState& game, Biome& outBiome, DungeonSize& outSize) {
    clearScreen();
    printHeader("🗺️  SELECT DUNGEON");
    printPlayerStats(game.getPlayer());
    
    std::cout << "\n🌍 Select Biome:\n";
    auto biomes = game.getAllBiomes();
    for (size_t i = 0; i < biomes.size(); i++) {
        std::cout << "  " << (i + 1) << ". " << game.getBiomeName(biomes[i]) << "\n";
    }
    
    std::cout << "\n0. Back to Main Menu\n";
    std::string choice;
    std::cout << "\nChoose a biome: ";
    std::getline(std::cin, choice);
    
    if (choice == "0") {
        return false;
    }
    
    try {
        int biomeIdx = std::stoi(choice) - 1;
        if (biomeIdx < 0 || biomeIdx >= static_cast<int>(biomes.size())) {
            return false;
        }
        
        outBiome = biomes[biomeIdx];
        
        // Now select size
        clearScreen();
        printHeader("🗺️  " + game.getBiomeName(outBiome) + " - SELECT SIZE");
        printPlayerStats(game.getPlayer());
        
        std::cout << "\n📏 Select Dungeon Size:\n";
        auto sizes = game.getAllDungeonSizes();
        for (size_t i = 0; i < sizes.size(); i++) {
            auto info = game.getDungeonSizeInfo(sizes[i]);
            std::cout << "  " << (i + 1) << ". " << info.displayName 
                     << " (" << info.floors << " floors, " 
                     << info.difficultyMultiplier << "x difficulty)\n";
        }
        
        std::cout << "\n0. Back\n";
        std::cout << "\nChoose a size: ";
        std::getline(std::cin, choice);
        
        if (choice == "0") {
            return false;
        }
        
        int sizeIdx = std::stoi(choice) - 1;
        if (sizeIdx < 0 || sizeIdx >= static_cast<int>(sizes.size())) {
            return false;
        }
        
        outSize = sizes[sizeIdx];
        return true;
        
    } catch (...) {
        return false;
    }
}

void combatMenu(GameState& game) {
    while (game.getCurrentEnemy() && game.getCurrentEnemy()->isAlive() && 
           game.getPlayer().isAlive() && game.isInDungeon()) {
        
        clearScreen();
        auto sizeInfo = game.getDungeonSizeInfo(game.getCurrentDungeonSize());
        printHeader("⚔️  COMBAT - " + game.getBiomeName(game.getCurrentBiome()) + 
                   " Floor " + std::to_string(game.getCurrentFloor()) + 
                   "/" + std::to_string(sizeInfo.floors));
        printPlayerStats(game.getPlayer());
        printEnemyStats(*game.getCurrentEnemy());
        
        std::cout << "\n⚔️  Combat Options:\n";
        std::cout << "  1. Attack\n";
        std::cout << "  2. Auto Battle (toggle)\n";
        std::cout << "  3. Flee (return to town)\n";
        
        std::string choice;
        if (game.isAutoBattle()) {
            std::cout << "\n⏩ Auto Battle ON - Fighting automatically...\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            choice = "1";
        } else {
            std::cout << "\nChoose an option: ";
            std::getline(std::cin, choice);
        }
        
        if (choice == "1") {
            auto result = game.attackEnemy();
            
            if (!game.isAutoBattle()) {
                std::cout << "\n💥 You dealt " << result.playerDamage << " damage!\n";
                
                if (result.enemyDefeated) {
                    std::cout << "🎉 Enemy defeated! +" << game.getCurrentEnemy()->goldReward 
                             << " gold, +" << game.getCurrentEnemy()->expReward << " exp\n";
                    
                    if (result.dungeonCompleted) {
                        std::cout << "\n🏆 DUNGEON COMPLETED! 🏆\n";
                        std::cout << "\nPress Enter to continue...";
                        std::cin.get();
                        return;
                    } else if (result.floorCleared) {
                        std::cout << "\n✨ Floor " << (game.getCurrentFloor() - 1) 
                                 << " cleared! Healing 30%...\n";
                        std::cout << "\nPress Enter to continue to next floor...";
                        std::cin.get();
                    }
                } else {
                    if (result.enemyDamage > 0) {
                        std::cout << "💔 Enemy dealt " << result.enemyDamage << " damage!\n";
                    }
                    
                    if (result.playerDied) {
                        std::cout << "\n💀 You have been defeated! Returning to town...\n";
                        std::cout << "\nPress Enter to continue...";
                        std::cin.get();
                        return;
                    }
                    
                    std::cout << "\nPress Enter to continue...";
                    std::cin.get();
                }
            }
        } else if (choice == "2") {
            game.toggleAutoBattle();
            std::string status = game.isAutoBattle() ? "ON" : "OFF";
            std::cout << "\n⏩ Auto Battle: " << status << "\n";
            if (!game.isAutoBattle()) {
                std::cout << "\nPress Enter to continue...";
                std::cin.get();
            }
        } else if (choice == "3") {
            game.fleeDungeon();
            return;
        }
    }
}

void upgradeMenu(GameState& game) {
    while (true) {
        clearScreen();
        printHeader("⬆️  UPGRADE STATS");
        printPlayerStats(game.getPlayer());
        
        std::cout << "\n💰 Upgrades Available:\n";
        std::cout << "  1. Max Health +20 (Cost: " << game.getUpgradeCost("health") << " gold)\n";
        std::cout << "  2. Attack +5 (Cost: " << game.getUpgradeCost("attack") << " gold)\n";
        std::cout << "  3. Defense +2 (Cost: " << game.getUpgradeCost("defense") << " gold)\n";
        std::cout << "\n  0. Back to Main Menu\n";
        
        std::string choice;
        std::cout << "\nChoose an upgrade: ";
        std::getline(std::cin, choice);
        
        if (choice == "0") {
            return;
        } else if (choice == "1") {
            if (game.upgradeStat("health")) {
                std::cout << "\n✅ Health upgraded!\n";
            } else {
                std::cout << "\n❌ Not enough gold!\n";
            }
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        } else if (choice == "2") {
            if (game.upgradeStat("attack")) {
                std::cout << "\n✅ Attack upgraded!\n";
            } else {
                std::cout << "\n❌ Not enough gold!\n";
            }
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        } else if (choice == "3") {
            if (game.upgradeStat("defense")) {
                std::cout << "\n✅ Defense upgraded!\n";
            } else {
                std::cout << "\n❌ Not enough gold!\n";
            }
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }
    }
}

void statisticsMenu(const GameState& game) {
    clearScreen();
    printHeader("📈 STATISTICS");
    printPlayerStats(game.getPlayer());
    
    std::cout << "\n🏆 Achievements:\n";
    std::cout << "  Total Floors Cleared: " << game.getPlayer().floorsCleared << "\n";
    std::cout << "  Total Dungeons Completed: " << game.getPlayer().dungeonsCompleted << "\n";
    std::cout << "  Current Level: " << game.getPlayer().level << "\n";
    
    std::cout << "\nPress Enter to return...";
    std::cin.get();
}
