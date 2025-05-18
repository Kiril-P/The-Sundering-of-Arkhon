#ifndef COMBAT_H
#define COMBAT_H

#include "player.h" // Needs player data

#define MAX_ENEMY_NAME_LENGTH 50
#define MAX_ENEMY_SPELLS 5 // If enemies can cast spells

// Forward declaration from player.h if not directly included, or ensure player.h is included before combat.h where used
// typedef struct Player Player;

// Enemy Definition
typedef struct Enemy {
    char name[MAX_ENEMY_NAME_LENGTH];
    int currentHp;
    int maxHp;
    int baseAttack;
    int baseDefense;
    // char spells[MAX_ENEMY_SPELLS][MAX_SPELL_NAME_LENGTH]; // Example if enemies have spells
    // int numSpells;
    char description[256]; // Description for when encountered
    // Add flags for special abilities, resistances, weaknesses etc.
} Enemy;

// Function Prototypes

/**
 * @brief Initiates and manages a combat encounter between the player and an enemy.
 * 
 * @param player Pointer to the player struct.
 * @param enemy Pointer to the enemy struct for this encounter.
 * @return Returns 1 if player wins, 0 if player flees, -1 if player loses.
 */
int startCombat(Player *player, Enemy *enemy);

/**
 * @brief Creates and returns an enemy instance by name.
 * This will be used to spawn enemies for encounters.
 * 
 * @param enemyName The name of the enemy type to create.
 * @param targetEnemy Pointer to an Enemy struct to populate.
 * @return Returns 1 if enemy was successfully created, 0 otherwise.
 */
int getEnemyByName(const char *enemyName, Enemy *targetEnemy);

/**
 * @brief Initiates and manages the unique final boss battle with Arkhon the Blight.
 * This function implements the special timed spellcasting mechanics.
 * 
 * @param player Pointer to the player struct.
 * @param boss Pointer to the Arkhon the Blight enemy struct.
 * @return Returns 1 if player wins, -1 if player loses. (Fleeing is not an option)
 */
int startFinalBossBattle(Player *player, Enemy *boss);

// Utility for timed spell casting
int castSpellTimed(const char *correctSpell, int timeLimitSeconds);

// Specific spell effect functions (can be expanded)
// void applyFlameSpark(Player *caster, Enemy *target);
// void applyShieldOfDawn(Player *caster);

#endif // COMBAT_H 