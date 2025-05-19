#ifndef PLAYER_H
#define PLAYER_H

#define MAX_SPELLS 10
#define MAX_SPELL_NAME_LENGTH 50
#define MAX_INVENTORY_ITEMS 10
#define MAX_ITEM_NAME_LENGTH 50

/* Progression Flags for tracking game state */
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
#define FLAG_ASKED_FIONA_ABOUT_ARKHON 8192 // For tracking if player asked Fiona about Arkhon
#define FLAG_MET_MARIS_FIRST_TIME 16384 // For tracking the first conversation with Elder Maris
#define FLAG_DIREFANG_SPIDER_DEFEATED 32768 // For tracking if the Direfang Spider is defeated
#define FLAG_DIREFANG_PENDANT_VISIBLE 65536 // Direfang Pendant is visible
#define FLAG_DIREFANG_PENDANT_COLLECTED 131072 // Direfang Pendant has been collected
#define FLAG_SHOPKEEPER_MET_FIRST_TIME 262144 // For tracking the first proper meeting with the shopkeeper
#define FLAG_RECEIVED_SHOP_POTION 524288 // For tracking if the free shop potion was received
#define FLAG_GROVE_PUZZLE_SOLVED 1048576 // For tracking if the Whispering Grove riddle was solved
#define FLAG_GROVE_COMPLETED_FOR_CAVERNS_QUEST 2097152 // Grove done, ready for Caverns quest
#define FLAG_CAVERNS_QUEST_GIVEN 4194304 // Caverns of Echoes quest/info has been given
#define FLAG_GRIZZIK_TALK_SUGGESTED 8388608 // Player has been prompted to talk to Grizzik
#define FLAG_GROVE_HEALING_RECEIVED 16777216 // Player has received the one-time heal in Whispering Grove

typedef struct {
    char name[MAX_SPELL_NAME_LENGTH];
} Spell;

typedef struct {
    char name[MAX_ITEM_NAME_LENGTH];
} Item;

typedef struct Player {
    int currentHp;
    int maxHp;
    Spell spellbook[MAX_SPELLS];
    int learnedSpellCount;
    Item inventory[MAX_INVENTORY_ITEMS];
    int inventoryItemCount;
    unsigned int progressionFlags;
    char currentLocation[MAX_ITEM_NAME_LENGTH];
} Player;

/* Player function prototypes */
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

#endif /* PLAYER_H */ 