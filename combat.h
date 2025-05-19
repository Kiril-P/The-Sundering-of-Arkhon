#ifndef COMBAT_H
#define COMBAT_H

#include "player.h"

#define MAX_ENEMY_NAME_LENGTH 50
#define MAX_ENEMY_SPELLS 5

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

/* Function Prototypes */

/* Handles combat between player and enemy
 * Returns: 1=win, 0=flee, -1=loss */
int startCombat(Player *player, Enemy *enemy);

/* Creates an enemy instance based on name 
 * Returns: 1=success, 0=failure */
int getEnemyByName(const char *enemyName, Enemy *targetEnemy);

/* Manages the final boss battle with special mechanics
 * Returns: 1=win, -1=loss (no fleeing allowed) */
int startFinalBossBattle(Player *player, Enemy *boss);

/* Implements timed spell casting mechanic */
int castSpellTimed(const char *correctSpell, int timeLimitSeconds);

// Specific spell effect functions (can be expanded)
// void applyFlameSpark(Player *caster, Enemy *target);
// void applyShieldOfDawn(Player *caster);

#endif /* COMBAT_H */ 