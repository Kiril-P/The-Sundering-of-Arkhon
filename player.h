#ifndef PLAYER_H
#define PLAYER_H

#define MAX_SPELLS 10
#define MAX_SPELL_NAME_LENGTH 50
#define MAX_INVENTORY_ITEMS 10
#define MAX_ITEM_NAME_LENGTH 50

// Progression Flags (can be expanded)
#define FLAG_STAFF_RETRIEVED 1
#define FLAG_GRIZZIK_MET 2
#define FLAG_LEARNED_FLAME_SPARK 4
#define FLAG_LEARNED_SHIELD_OF_DAWN 8
#define FLAG_LEARNED_LIGHTNING_ARC 16
#define FLAG_LEARNED_NATURES_EMBRACE 512
#define FLAG_LEARNED_SHADOWS_BANE 64
#define FLAG_DEFEATED_BLIGHT_STANDARD 128
#define FLAG_CROSSROADS_AMBUSH_DONE 256 // For the one-time goblin ambush at crossroads
#define FLAG_GROVE_PUZZLE_ATTEMPTED 1024 // For the Whispering Grove riddle
#define FLAG_CAVERNS_PUZZLE_SOLVED 2048 // For the Caverns of Echoes main puzzle
#define FLAG_CAVERNS_TROLL_DEFEATED 4096 // For defeating the Cave Troll in Caverns of Echoes
// ... add more flags for lore points, quest completions, etc.


typedef struct {
    char name[MAX_SPELL_NAME_LENGTH];
    // Add other spell properties if needed, e.g., damage, type (offense/defense)
} Spell;

typedef struct {
    char name[MAX_ITEM_NAME_LENGTH];
    // Add other item properties, e.g., description, effect
} Item;

typedef struct Player {
    int currentHp;
    int maxHp;
    Spell spellbook[MAX_SPELLS];
    int learnedSpellCount;
    Item inventory[MAX_INVENTORY_ITEMS];
    int inventoryItemCount;
    unsigned int progressionFlags; // Using unsigned int for bitmasking flags
    char currentLocation[MAX_ITEM_NAME_LENGTH]; // To store player's current location
} Player;

// Function prototypes
void initializePlayer(Player *player, const char *startLocation);
void displayPlayerStatus(const Player *player);
int hasSpell(const Player *player, const char *spellName);
void learnSpell(Player *player, const char *spellName);
int hasItem(const Player *player, const char *itemName);
void addItemToInventory(Player *player, const char *itemName);
void takeDamage(Player *player, int damageAmount);
void healPlayer(Player *player, int healAmount);
int checkProgressionFlag(const Player *player, unsigned int flag);
void setProgressionFlag(Player *player, unsigned int flag);

#endif // PLAYER_H 