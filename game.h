#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>
#include <memory>
#include <map>

// Forward declarations
class Enemy;
class Player;

// Enums
enum class Biome {
    FOREST,
    CAVE,
    DESERT,
    ICE,
    VOLCANO
};

enum class DungeonSize {
    SMALL,
    MEDIUM,
    LARGE,
    EPIC
};

// Dungeon size info structure
struct DungeonSizeInfo {
    std::string displayName;
    int floors;
    double difficultyMultiplier;
};

// Enemy class
class Enemy {
public:
    std::string name;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int goldReward;
    int expReward;
    
    Enemy(const std::string& n, int h, int atk, int def, int gold, int exp);
    bool isAlive() const;
    int takeDamage(int damage);
};

// Player class
class Player {
public:
    std::string name;
    int level;
    int health;
    int maxHealth;
    int attack;
    int defense;
    int gold;
    int experience;
    int expToNextLevel;
    int floorsCleared;
    int dungeonsCompleted;
    
    Player();
    bool isAlive() const;
    int takeDamage(int damage);
    void heal(int amount);
    void fullHeal();
    void gainExperience(int exp);
    void levelUp();
    bool canAfford(int cost) const;
    bool spendGold(int amount);
};

// Combat result structure
struct CombatResult {
    int playerDamage;
    bool enemyDefeated;
    int enemyDamage;
    bool playerDied;
    bool floorCleared;
    bool dungeonCompleted;
};

// Game state class
class GameState {
private:
    Player player;
    Biome currentBiome;
    DungeonSize currentDungeonSize;
    int currentFloor;
    std::shared_ptr<Enemy> currentEnemy;
    bool autoBattle;
    bool inDungeon;
    
    std::map<Biome, std::vector<std::string>> enemyTypes;
    std::map<DungeonSize, DungeonSizeInfo> dungeonSizeInfo;
    std::map<Biome, std::string> biomeNames;
    
    void initializeData();
    
public:
    bool gameRunning;
    
    GameState();
    
    // Getters
    Player& getPlayer();
    const Player& getPlayer() const;
    Biome getCurrentBiome() const;
    DungeonSize getCurrentDungeonSize() const;
    int getCurrentFloor() const;
    std::shared_ptr<Enemy> getCurrentEnemy() const;
    bool isAutoBattle() const;
    bool isInDungeon() const;
    
    // Game actions
    void startDungeon(Biome biome, DungeonSize size);
    void spawnEnemy();
    CombatResult attackEnemy();
    bool upgradeStat(const std::string& stat);
    int getUpgradeCost(const std::string& stat) const;
    void toggleAutoBattle();
    void fleeDungeon();
    
    // Save/Load
    bool saveGame(const std::string& filename = "save_game.json");
    bool loadGame(const std::string& filename = "save_game.json");
    
    // Utility
    std::string getBiomeName(Biome biome) const;
    DungeonSizeInfo getDungeonSizeInfo(DungeonSize size) const;
    std::vector<Biome> getAllBiomes() const;
    std::vector<DungeonSize> getAllDungeonSizes() const;
};

// UI functions
void clearScreen();
void printHeader(const std::string& text);
void printPlayerStats(const Player& player);
void printEnemyStats(const Enemy& enemy);

// Menu functions
std::string mainMenu(const GameState& game);
bool dungeonSelectionMenu(GameState& game, Biome& outBiome, DungeonSize& outSize);
void combatMenu(GameState& game);
void upgradeMenu(GameState& game);
void statisticsMenu(const GameState& game);

#endif // GAME_H
