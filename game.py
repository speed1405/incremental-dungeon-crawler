#!/usr/bin/env python3
"""
Incremental Dungeon Crawler
A text-based incremental dungeon crawler game with multiple biomes and dungeon sizes.
"""

import json
import os
import random
import time
from enum import Enum
from typing import List, Dict, Optional, Tuple


class Biome(Enum):
    """Biome types for dungeons"""
    FOREST = 0
    CAVE = 1
    DESERT = 2
    ICE = 3
    VOLCANO = 4


class DungeonSize(Enum):
    """Dungeon size types"""
    SMALL = 0
    MEDIUM = 1
    LARGE = 2
    EPIC = 3


class DungeonSizeInfo:
    """Information about a dungeon size"""
    def __init__(self, display_name: str, floors: int, difficulty_multiplier: float):
        self.display_name = display_name
        self.floors = floors
        self.difficulty_multiplier = difficulty_multiplier


class Enemy:
    """Enemy class"""
    def __init__(self, name: str, health: int, attack: int, defense: int, gold: int, exp: int):
        self.name = name
        self.health = health
        self.max_health = health
        self.attack = attack
        self.defense = defense
        self.gold_reward = gold
        self.exp_reward = exp
    
    def is_alive(self) -> bool:
        """Check if enemy is alive"""
        return self.health > 0
    
    def take_damage(self, damage: int) -> int:
        """Take damage and return actual damage dealt"""
        actual_damage = max(1, damage - self.defense)
        self.health = max(0, self.health - actual_damage)
        return actual_damage


class Player:
    """Player class"""
    def __init__(self):
        self.name = "Hero"
        self.level = 1
        self.health = 100
        self.max_health = 100
        self.attack = 10
        self.defense = 5
        self.gold = 0
        self.experience = 0
        self.exp_to_next_level = 100
        self.floors_cleared = 0
        self.dungeons_completed = 0
    
    def is_alive(self) -> bool:
        """Check if player is alive"""
        return self.health > 0
    
    def take_damage(self, damage: int) -> int:
        """Take damage and return actual damage dealt"""
        actual_damage = max(1, damage - self.defense)
        self.health = max(0, self.health - actual_damage)
        return actual_damage
    
    def heal(self, amount: int):
        """Heal the player"""
        self.health = min(self.max_health, self.health + amount)
    
    def full_heal(self):
        """Fully heal the player"""
        self.health = self.max_health
    
    def gain_experience(self, exp: int):
        """Gain experience and level up if needed"""
        self.experience += exp
        while self.experience >= self.exp_to_next_level:
            self.level_up()
    
    def level_up(self):
        """Level up the player"""
        self.experience -= self.exp_to_next_level
        self.level += 1
        self.max_health += 20
        self.health = self.max_health
        self.attack += 5
        self.defense += 2
        self.exp_to_next_level = int(self.exp_to_next_level * 1.5)
    
    def can_afford(self, cost: int) -> bool:
        """Check if player can afford a cost"""
        return self.gold >= cost
    
    def spend_gold(self, amount: int) -> bool:
        """Spend gold if possible"""
        if self.can_afford(amount):
            self.gold -= amount
            return True
        return False


class CombatResult:
    """Result of a combat action"""
    def __init__(self):
        self.player_damage = 0
        self.enemy_defeated = False
        self.enemy_damage = 0
        self.player_died = False
        self.floor_cleared = False
        self.dungeon_completed = False
        self.gold_reward = 0
        self.exp_reward = 0


class GameState:
    """Main game state class"""
    def __init__(self):
        self.player = Player()
        self.current_biome = Biome.FOREST
        self.current_dungeon_size = DungeonSize.SMALL
        self.current_floor = 0
        self.current_enemy: Optional[Enemy] = None
        self.auto_battle = False
        self.in_dungeon = False
        self.game_running = True
        
        # Initialize game data
        self.enemy_types: Dict[Biome, List[str]] = {
            Biome.FOREST: ["Goblin", "Wolf", "Bear", "Troll"],
            Biome.CAVE: ["Bat", "Spider", "Slime", "Golem"],
            Biome.DESERT: ["Scorpion", "Snake", "Mummy", "Sand Elemental"],
            Biome.ICE: ["Ice Sprite", "Frost Wolf", "Yeti", "Ice Dragon"],
            Biome.VOLCANO: ["Fire Imp", "Lava Golem", "Magma Worm", "Phoenix"]
        }
        
        self.dungeon_size_info: Dict[DungeonSize, DungeonSizeInfo] = {
            DungeonSize.SMALL: DungeonSizeInfo("Small", 5, 1.0),
            DungeonSize.MEDIUM: DungeonSizeInfo("Medium", 10, 1.5),
            DungeonSize.LARGE: DungeonSizeInfo("Large", 20, 2.0),
            DungeonSize.EPIC: DungeonSizeInfo("Epic", 50, 3.0)
        }
        
        self.biome_names: Dict[Biome, str] = {
            Biome.FOREST: "Forest",
            Biome.CAVE: "Cave",
            Biome.DESERT: "Desert",
            Biome.ICE: "Ice Cavern",
            Biome.VOLCANO: "Volcano"
        }
    
    def get_biome_name(self, biome: Biome) -> str:
        """Get the display name of a biome"""
        return self.biome_names[biome]
    
    def get_dungeon_size_info(self, size: DungeonSize) -> DungeonSizeInfo:
        """Get information about a dungeon size"""
        return self.dungeon_size_info[size]
    
    def get_all_biomes(self) -> List[Biome]:
        """Get all available biomes"""
        return [Biome.FOREST, Biome.CAVE, Biome.DESERT, Biome.ICE, Biome.VOLCANO]
    
    def get_all_dungeon_sizes(self) -> List[DungeonSize]:
        """Get all available dungeon sizes"""
        return [DungeonSize.SMALL, DungeonSize.MEDIUM, DungeonSize.LARGE, DungeonSize.EPIC]
    
    def start_dungeon(self, biome: Biome, size: DungeonSize):
        """Start a new dungeon run"""
        self.current_biome = biome
        self.current_dungeon_size = size
        self.current_floor = 1
        self.in_dungeon = True
        self.player.full_heal()
        self.spawn_enemy()
    
    def spawn_enemy(self):
        """Spawn a new enemy for the current floor"""
        if not self.in_dungeon:
            return
        
        # Base enemy stats scale with floor
        floor_multiplier = 1.0 + (self.current_floor - 1) * 0.2
        difficulty = self.dungeon_size_info[self.current_dungeon_size].difficulty_multiplier
        
        base_health = int(50 * floor_multiplier * difficulty)
        base_attack = int(8 * floor_multiplier * difficulty)
        base_defense = int(3 * floor_multiplier * difficulty)
        base_gold = int(10 * floor_multiplier * difficulty)
        base_exp = int(20 * floor_multiplier * difficulty)
        
        # Select random enemy type
        types = self.enemy_types[self.current_biome]
        enemy_name = random.choice(types)
        
        # Boss on final floor
        if self.current_floor == self.dungeon_size_info[self.current_dungeon_size].floors:
            enemy_name = self.biome_names[self.current_biome] + " Boss"
            base_health = int(base_health * 2.5)
            base_attack = int(base_attack * 1.5)
            base_defense = int(base_defense * 1.5)
            base_gold = int(base_gold * 3)
            base_exp = int(base_exp * 3)
        
        self.current_enemy = Enemy(enemy_name, base_health, base_attack, 
                                   base_defense, base_gold, base_exp)
    
    def attack_enemy(self) -> CombatResult:
        """Player attacks the enemy"""
        result = CombatResult()
        
        if not self.current_enemy or not self.current_enemy.is_alive():
            return result
        
        # Player attacks
        result.player_damage = self.current_enemy.take_damage(self.player.attack)
        
        # Check if enemy is defeated
        if not self.current_enemy.is_alive():
            result.enemy_defeated = True
            result.gold_reward = self.current_enemy.gold_reward
            result.exp_reward = self.current_enemy.exp_reward
            self.player.gold += result.gold_reward
            self.player.gain_experience(result.exp_reward)
            self.player.floors_cleared += 1
            
            # Check if dungeon is completed
            if self.current_floor >= self.dungeon_size_info[self.current_dungeon_size].floors:
                result.dungeon_completed = True
                self.player.dungeons_completed += 1
                self.current_floor = 0
                self.current_enemy = None
                self.in_dungeon = False
                self.auto_battle = False
                return result
            else:
                # Advance to next floor
                self.current_floor += 1
                result.floor_cleared = True
                self.player.heal(int(self.player.max_health * 0.3))
                self.spawn_enemy()
                return result
        
        # Enemy attacks back
        if self.current_enemy.is_alive():
            result.enemy_damage = self.player.take_damage(self.current_enemy.attack)
            
            if not self.player.is_alive():
                result.player_died = True
                # Reset to town
                self.current_floor = 0
                self.current_enemy = None
                self.in_dungeon = False
                self.auto_battle = False
                self.player.full_heal()
        
        return result
    
    def upgrade_stat(self, stat: str) -> bool:
        """Upgrade a player stat"""
        cost = self.get_upgrade_cost(stat)
        
        if cost == 0 or not self.player.spend_gold(cost):
            return False
        
        if stat == "health":
            self.player.max_health += 20
            self.player.health = self.player.max_health
        elif stat == "attack":
            self.player.attack += 5
        elif stat == "defense":
            self.player.defense += 2
        
        return True
    
    def get_upgrade_cost(self, stat: str) -> int:
        """Get the cost to upgrade a stat"""
        if stat == "health":
            upgrades = max(0, self.player.max_health // 20 - 5)
            return int(50 * (1.5 ** upgrades))
        elif stat == "attack":
            upgrades = max(0, self.player.attack // 5 - 2)
            return int(100 * (1.5 ** upgrades))
        elif stat == "defense":
            upgrades = max(0, self.player.defense // 2 - 2)
            return int(80 * (1.5 ** upgrades))
        return 0
    
    def toggle_auto_battle(self):
        """Toggle auto battle mode"""
        self.auto_battle = not self.auto_battle
    
    def flee_dungeon(self):
        """Flee from the dungeon"""
        self.current_floor = 0
        self.current_enemy = None
        self.in_dungeon = False
        self.auto_battle = False
        self.player.full_heal()
    
    def save_game(self, filename: str = "save_game.json") -> bool:
        """Save the game to a JSON file"""
        try:
            data = {
                "player": {
                    "name": self.player.name,
                    "level": self.player.level,
                    "health": self.player.health,
                    "maxHealth": self.player.max_health,
                    "attack": self.player.attack,
                    "defense": self.player.defense,
                    "gold": self.player.gold,
                    "experience": self.player.experience,
                    "expToNextLevel": self.player.exp_to_next_level,
                    "floorsCleared": self.player.floors_cleared,
                    "dungeonsCompleted": self.player.dungeons_completed
                },
                "currentFloor": self.current_floor,
                "autoBattle": self.auto_battle,
                "inDungeon": self.in_dungeon
            }
            
            with open(filename, 'w') as f:
                json.dump(data, f, indent=2)
            return True
        except Exception:
            return False
    
    def load_game(self, filename: str = "save_game.json") -> bool:
        """Load the game from a JSON file"""
        try:
            with open(filename, 'r') as f:
                data = json.load(f)
            
            player_data = data["player"]
            self.player.name = player_data["name"]
            self.player.level = player_data["level"]
            self.player.health = player_data["health"]
            self.player.max_health = player_data["maxHealth"]
            self.player.attack = player_data["attack"]
            self.player.defense = player_data["defense"]
            self.player.gold = player_data["gold"]
            self.player.experience = player_data["experience"]
            self.player.exp_to_next_level = player_data["expToNextLevel"]
            self.player.floors_cleared = player_data["floorsCleared"]
            self.player.dungeons_completed = player_data["dungeonsCompleted"]
            
            # Restore game state (note: we don't restore in_dungeon to avoid inconsistent state)
            # Players always start in town after loading
            self.current_floor = 0
            self.auto_battle = False
            self.in_dungeon = False
            
            return True
        except Exception:
            return False


# UI Functions
def clear_screen():
    """Clear the terminal screen"""
    os.system('cls' if os.name == 'nt' else 'clear')


def print_header(text: str):
    """Print a formatted header"""
    print("\n" + "=" * 60)
    print(f"  {text}")
    print("=" * 60)


def print_player_stats(player: Player):
    """Print player statistics"""
    print("\n📊 Player Stats:")
    print(f"  Level: {player.level} | HP: {player.health}/{player.max_health}")
    print(f"  Attack: {player.attack} | Defense: {player.defense}")
    print(f"  Gold: {player.gold} | EXP: {player.experience}/{player.exp_to_next_level}")
    print(f"  Floors Cleared: {player.floors_cleared} | Dungeons: {player.dungeons_completed}")


def print_enemy_stats(enemy: Enemy):
    """Print enemy statistics"""
    print(f"\n⚔️  Enemy: {enemy.name}")
    print(f"  HP: {enemy.health}/{enemy.max_health}")
    print(f"  Attack: {enemy.attack} | Defense: {enemy.defense}")


# Menu Functions
def main_menu(game: GameState) -> str:
    """Display main menu and get user choice"""
    clear_screen()
    print_header("🏰 INCREMENTAL DUNGEON CRAWLER 🏰")
    print_player_stats(game.player)
    
    print("\n📜 Main Menu:")
    print("  1. Enter Dungeon")
    print("  2. Upgrade Stats")
    print("  3. View Statistics")
    print("  4. Save Game")
    print("  5. Load Game")
    print("  6. Exit")
    
    choice = input("\nChoose an option: ")
    return choice


def dungeon_selection_menu(game: GameState) -> Tuple[Optional[Biome], Optional[DungeonSize]]:
    """Display dungeon selection menu"""
    clear_screen()
    print_header("🗺️  SELECT DUNGEON")
    print_player_stats(game.player)
    
    print("\n🌍 Select Biome:")
    biomes = game.get_all_biomes()
    for i, biome in enumerate(biomes):
        print(f"  {i + 1}. {game.get_biome_name(biome)}")
    
    print("\n0. Back to Main Menu")
    choice = input("\nChoose a biome: ")
    
    if choice == "0":
        return None, None
    
    try:
        biome_idx = int(choice) - 1
        if biome_idx < 0 or biome_idx >= len(biomes):
            return None, None
        
        selected_biome = biomes[biome_idx]
        
        # Now select size
        clear_screen()
        print_header(f"🗺️  {game.get_biome_name(selected_biome)} - SELECT SIZE")
        print_player_stats(game.player)
        
        print("\n📏 Select Dungeon Size:")
        sizes = game.get_all_dungeon_sizes()
        for i, size in enumerate(sizes):
            info = game.get_dungeon_size_info(size)
            print(f"  {i + 1}. {info.display_name} ({info.floors} floors, {info.difficulty_multiplier}x difficulty)")
        
        print("\n0. Back")
        choice = input("\nChoose a size: ")
        
        if choice == "0":
            return None, None
        
        size_idx = int(choice) - 1
        if size_idx < 0 or size_idx >= len(sizes):
            return None, None
        
        selected_size = sizes[size_idx]
        return selected_biome, selected_size
    
    except (ValueError, IndexError):
        return None, None


def combat_menu(game: GameState):
    """Display combat menu and handle combat"""
    while (game.current_enemy and game.current_enemy.is_alive() and 
           game.player.is_alive() and game.in_dungeon):
        
        clear_screen()
        size_info = game.get_dungeon_size_info(game.current_dungeon_size)
        print_header(f"⚔️  COMBAT - {game.get_biome_name(game.current_biome)} "
                    f"Floor {game.current_floor}/{size_info.floors}")
        print_player_stats(game.player)
        print_enemy_stats(game.current_enemy)
        
        print("\n⚔️  Combat Options:")
        print("  1. Attack")
        print("  2. Auto Battle (toggle)")
        print("  3. Flee (return to town)")
        
        if game.auto_battle:
            print("\n⏩ Auto Battle ON - Fighting automatically...")
            time.sleep(0.5)
            choice = "1"
        else:
            choice = input("\nChoose an option: ")
        
        if choice == "1":
            result = game.attack_enemy()
            
            if not game.auto_battle:
                print(f"\n💥 You dealt {result.player_damage} damage!")
                
                if result.enemy_defeated:
                    print(f"🎉 Enemy defeated! +{result.gold_reward} gold, "
                         f"+{result.exp_reward} exp")
                    
                    if result.dungeon_completed:
                        print("\n🏆 DUNGEON COMPLETED! 🏆")
                        input("\nPress Enter to continue...")
                        return
                    elif result.floor_cleared:
                        print(f"\n✨ Floor {game.current_floor - 1} cleared! Healing 30%...")
                        input("\nPress Enter to continue to next floor...")
                else:
                    if result.enemy_damage > 0:
                        print(f"💔 Enemy dealt {result.enemy_damage} damage!")
                    
                    if result.player_died:
                        print("\n💀 You have been defeated! Returning to town...")
                        input("\nPress Enter to continue...")
                        return
                    
                    input("\nPress Enter to continue...")
        
        elif choice == "2":
            game.toggle_auto_battle()
            status = "ON" if game.auto_battle else "OFF"
            print(f"\n⏩ Auto Battle: {status}")
            if not game.auto_battle:
                input("\nPress Enter to continue...")
        
        elif choice == "3":
            game.flee_dungeon()
            return


def upgrade_menu(game: GameState):
    """Display upgrade menu"""
    while True:
        clear_screen()
        print_header("⬆️  UPGRADE STATS")
        print_player_stats(game.player)
        
        print("\n💰 Upgrades Available:")
        print(f"  1. Max Health +20 (Cost: {game.get_upgrade_cost('health')} gold)")
        print(f"  2. Attack +5 (Cost: {game.get_upgrade_cost('attack')} gold)")
        print(f"  3. Defense +2 (Cost: {game.get_upgrade_cost('defense')} gold)")
        print("\n  0. Back to Main Menu")
        
        choice = input("\nChoose an upgrade: ")
        
        if choice == "0":
            return
        elif choice == "1":
            if game.upgrade_stat("health"):
                print("\n✅ Health upgraded!")
            else:
                print("\n❌ Not enough gold!")
            input("\nPress Enter to continue...")
        elif choice == "2":
            if game.upgrade_stat("attack"):
                print("\n✅ Attack upgraded!")
            else:
                print("\n❌ Not enough gold!")
            input("\nPress Enter to continue...")
        elif choice == "3":
            if game.upgrade_stat("defense"):
                print("\n✅ Defense upgraded!")
            else:
                print("\n❌ Not enough gold!")
            input("\nPress Enter to continue...")


def statistics_menu(game: GameState):
    """Display statistics menu"""
    clear_screen()
    print_header("📈 STATISTICS")
    print_player_stats(game.player)
    
    print("\n🏆 Achievements:")
    print(f"  Total Floors Cleared: {game.player.floors_cleared}")
    print(f"  Total Dungeons Completed: {game.player.dungeons_completed}")
    print(f"  Current Level: {game.player.level}")
    
    input("\nPress Enter to return...")


def main():
    """Main game loop"""
    game = GameState()
    
    # Try to load saved game
    if os.path.exists("save_game.json"):
        response = input("Found saved game. Load it? (y/n): ")
        if response.lower() == "y":
            if game.load_game():
                print("Game loaded successfully!")
            else:
                print("Failed to load game. Starting new game...")
            input("\nPress Enter to continue...")
    
    while game.game_running:
        choice = main_menu(game)
        
        if choice == "1":
            # Enter dungeon
            selected_biome, selected_size = dungeon_selection_menu(game)
            if selected_biome is not None and selected_size is not None:
                game.start_dungeon(selected_biome, selected_size)
                combat_menu(game)
        
        elif choice == "2":
            # Upgrade stats
            upgrade_menu(game)
        
        elif choice == "3":
            # View statistics
            statistics_menu(game)
        
        elif choice == "4":
            # Save game
            if game.save_game():
                print("\n💾 Game saved successfully!")
            else:
                print("\n❌ Failed to save game!")
            input("\nPress Enter to continue...")
        
        elif choice == "5":
            # Load game
            if game.load_game():
                print("\n💾 Game loaded successfully!")
            else:
                print("\n❌ No save file found or failed to load!")
            input("\nPress Enter to continue...")
        
        elif choice == "6":
            # Exit
            print("\n👋 Thanks for playing!")
            game.game_running = False


if __name__ == "__main__":
    main()
