#include "combat.h"
#include "player.h" // For Player, Spell, Item, MAX_SPELL_NAME_LENGTH, hasSpell, takeDamage etc.
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // For rand(), srand()
#include <time.h>   // For time() in castSpell
#include <strings.h> // For strcasecmp

// This function was previously in main.c
// It's a core mechanic for spellcasting in combat and potentially puzzles.
int castSpellTimed(const char *correctSpell, int timeLimitSeconds) {
    char input[100];
    time_t start_time, end_time;
    time(&start_time);

    printf("Recite the incantation: %s\n> ", correctSpell);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; // Remove newline

    time(&end_time);
    if (difftime(end_time, start_time) > timeLimitSeconds) {
        printf("Too slow! The magic fizzles.\n");
        return 0; // Too slow
    }

    // Case-insensitive comparison for incantations might be more user-friendly
    if (strcasecmp(input, correctSpell) == 0) {
        // Success message is now contextual in startCombat
        return 1; // Correct spell cast
    }

    printf("Incorrect incantation! The spell fails.\n");
    return 0; // Incorrect spell
}

int getEnemyByName(const char *enemyName, Enemy *targetEnemy) {
    if (!targetEnemy || !enemyName) return 0;
    memset(targetEnemy, 0, sizeof(Enemy)); // Clear the struct first

    if (strcasecmp(enemyName, "Direfang_Spider") == 0) {
        strncpy(targetEnemy->name, "Direfang Spider", MAX_ENEMY_NAME_LENGTH - 1);
        targetEnemy->maxHp = 40;
        targetEnemy->currentHp = targetEnemy->maxHp;
        targetEnemy->baseAttack = 12;
        targetEnemy->baseDefense = 5;
        strncpy(targetEnemy->description, "A large, venomous spider from Direfang Dungeon. Its fangs drip with poison.", sizeof(targetEnemy->description) - 1);
        return 1;
    } else if (strcasecmp(enemyName, "Goblin_Scout") == 0) {
        strncpy(targetEnemy->name, "Goblin Scout", MAX_ENEMY_NAME_LENGTH - 1);
        targetEnemy->maxHp = 25;
        targetEnemy->currentHp = targetEnemy->maxHp;
        targetEnemy->baseAttack = 8;
        targetEnemy->baseDefense = 3;
        strncpy(targetEnemy->description, "A small, wiry goblin scout, quick and armed with a rusty dagger.", sizeof(targetEnemy->description) - 1);
        return 1;
    } else if (strcasecmp(enemyName, "Whispering_Wraith") == 0) {
        strncpy(targetEnemy->name, "Whispering Wraith", MAX_ENEMY_NAME_LENGTH - 1);
        targetEnemy->maxHp = 60; // Wraiths are tougher
        targetEnemy->currentHp = targetEnemy->maxHp;
        targetEnemy->baseAttack = 15; // Higher damage
        targetEnemy->baseDefense = 8; // More resilient
        strncpy(targetEnemy->description, "A spectral figure from the Whispering Grove, its touch chills to the bone.", sizeof(targetEnemy->description) - 1);
        return 1;
    } else if (strcasecmp(enemyName, "Cave_Troll") == 0) {
        strncpy(targetEnemy->name, "Cave Troll", MAX_ENEMY_NAME_LENGTH - 1);
        targetEnemy->maxHp = 100; // Very tough
        targetEnemy->currentHp = targetEnemy->maxHp;
        targetEnemy->baseAttack = 20; // Hits hard
        targetEnemy->baseDefense = 10; // Thick hide
        strncpy(targetEnemy->description, "A hulking Cave Troll from the Caverns of Echoes, brandishing a massive club.", sizeof(targetEnemy->description) - 1);
        return 1;
    } else if (strcasecmp(enemyName, "Arkhon_The_Blight") == 0) {
        strncpy(targetEnemy->name, "Arkhon the Blight", MAX_ENEMY_NAME_LENGTH - 1);
        targetEnemy->maxHp = 250; // Final Boss HP
        targetEnemy->currentHp = targetEnemy->maxHp;
        targetEnemy->baseAttack = 25; // Final Boss Attack
        targetEnemy->baseDefense = 15; // Final Boss Defense
        strncpy(targetEnemy->description, "Your dark reflection, Arkhon the Blight, radiates immense power and malice.", sizeof(targetEnemy->description) - 1);
        return 1;
    }
    // Add more enemies here as defined in GameFlow/GameDesign for various dungeons

    return 0; // Enemy type not found
}

// This function was previously handleCombat in main.c
int startCombat(Player *player, Enemy *enemy) {
    if (!player || !enemy) return -1; // Error case

    printf("\n--- Encounter! --- \n%s\n", enemy->description);
    printf("You face %s (HP: %d)!\n", enemy->name, enemy->currentHp);

    int playerShieldTurns = 0; // Duration for Shield of Dawn effect

    char combatChoice[100];
    while(player->currentHp > 0 && enemy->currentHp > 0) {
        printf("\nYour HP: %d/%d | %s's HP: %d/%d", player->currentHp, player->maxHp, enemy->name, enemy->currentHp, enemy->maxHp);
        if (playerShieldTurns > 0) printf(" (Shielded)");
        printf("\nCombat options: attack, spell <name>, status, flee\n> ");
        fgets(combatChoice, sizeof(combatChoice), stdin);
        combatChoice[strcspn(combatChoice, "\n")] = 0;

        if (strcasecmp(combatChoice, "attack") == 0) {
            // Simple physical attack
            int damageDealt = 10 + (rand() % 6); // Base player attack: 10-15 damage
            damageDealt -= enemy->baseDefense;
            if (damageDealt < 0) damageDealt = 0;
            printf("You strike %s with your staff for %d damage!\n", enemy->name, damageDealt);
            enemy->currentHp -= damageDealt;
        } else if (strncasecmp(combatChoice, "spell ", 6) == 0) {
            char spellToUse[MAX_SPELL_NAME_LENGTH];
            sscanf(combatChoice + 6, "%49s", spellToUse);
            if (hasSpell(player, spellToUse)) {
                if (strcasecmp(spellToUse, "Flame_Spark") == 0) {
                    if (castSpellTimed("Ignis Minor", 10)) { // Incantation from GameDesignDoc
                        int spellDamage = 20 + (rand() % 11); // Flame Spark: 20-30 damage
                        printf("A brilliant spark of flame erupts from your staff, scorching %s for %d damage!\n", enemy->name, spellDamage);
                        enemy->currentHp -= spellDamage;
                    } else {
                        printf("Your Flame Spark fizzles due to incorrect or slow incantation!\n");
                    }
                } else if (strcasecmp(spellToUse, "Shield_of_Dawn") == 0) {
                     if (castSpellTimed("Aegis Lucis", 8)) { // Incantation from GameDesignDoc
                        printf("A shimmering shield of light envelops you, bolstering your defense!\n");
                        playerShieldTurns = 2; // Shield lasts for 2 enemy attacks
                     } else {
                        printf("Your Shield of Dawn fails to materialize!\n");
                     }
                } else if (strcasecmp(spellToUse, "Lightning_Arc") == 0) {
                    if (castSpellTimed("Fulmen Arcana", 12)) { // Placeholder incantation
                        int spellDamage = 35 + (rand() % 16); // Lightning Arc: 35-50 damage
                        printf("A crackling arc of lightning leaps from your fingertips, striking %s for %d damage!\n", enemy->name, spellDamage);
                        enemy->currentHp -= spellDamage;
                    } else {
                        printf("Your Lightning Arc dissipates weakly!\n");
                    }
                } else if (strcasecmp(spellToUse, "Natures_Embrace") == 0) {
                    if (castSpellTimed("Terra Sanatio", 10)) { // Placeholder incantation
                        int healAmount = 30 + (rand() % 21); // Nature's Embrace: 30-50 HP heal
                        healPlayer(player, healAmount); // Using healPlayer from player.c
                    } else {
                        printf("The healing energies of Nature's Embrace fail to coalesce!\n");
                    }
                }
                // Add Shadows_Bane later - GameFlow says it dispels forcefield, not a direct combat spell unless vs specific enemies.
                else {
                    printf("You can't use '%s' effectively in this combat right now, or it's not an offensive/defensive spell for this context.\n", spellToUse);
                }
            } else {
                printf("You don't know the spell '%s'.\n", spellToUse);
            }
        } else if (strcasecmp(combatChoice, "flee") == 0) {
            // Bosses might prevent fleeing
            if (strcasecmp(enemy->name, "Arkhon the Blight") == 0) {
                printf("Arkhon the Blight mocks your attempt: 'You cannot escape your own shadow!' Fleeing is impossible!\n");
            } else {
                printf("You attempt to flee...\n");
                if ((rand() % 100) < 75) { // 75% chance to flee non-bosses
                    printf("You successfully escape from %s!\n", enemy->name);
                    return 0; // Player fled
                } else {
                    printf("You couldn't find an opening to escape!\n");
                }
            }
        } else if (strcasecmp(combatChoice, "status") == 0) {
            displayPlayerStatus(player);
            continue; // Does not consume a turn
        }else {
            printf("Invalid combat command. Options: attack, spell <name>, status, flee.\n");
            continue; // Does not consume a turn if invalid command
        }

        if (enemy->currentHp <= 0) {
            printf("\n%s has been defeated!\n", enemy->name);
            // TODO: Add specific rewards based on enemy (lore, items, spell scrolls)
            if (strcasecmp(enemy->name, "Direfang Spider") == 0) {
                printf("The spider dissolves into dust, leaving behind a strange, pulsating ichor.\n");
                // addItemToInventory(player, "Spider_Ichor"); // Example item
            }
            return 1; // Player won
        }

        // Enemy Attack Phase
        if (enemy->currentHp > 0) {
            printf("\n%s prepares to attack!\n", enemy->name);
            // TODO: Implement enemy spellcasting or special attacks for variety
            int enemyDamage = enemy->baseAttack - (playerShieldTurns > 0 ? 5 : 0); // Simple shield effect: flat damage reduction
            if (playerShieldTurns > 0) {
                printf("Your shield absorbs some of the blow!\n");
                playerShieldTurns--;
            }
            if (enemyDamage < 0) enemyDamage = 1; // Minimum 1 damage if shielded heavily
            
            printf("%s attacks you for %d damage!\n", enemy->name, enemyDamage);
            takeDamage(player, enemyDamage); // takeDamage is from player.c

            if (player->currentHp <= 0) {
                // Death message is handled by takeDamage in player.c
                return -1; // Player lost
            }
        }
    }
    // Should not be reached if combat ends correctly with return statements above.
    if (player->currentHp <=0) return -1;
    if (enemy->currentHp <=0) return 1;
    return 0; // Default to flee or stalemate if loop exits unexpectedly
}

// --- Final Boss Battle Implementation ---
int startFinalBossBattle(Player *player, Enemy *boss) {
    if (!player || !boss || strcasecmp(boss->name, "Arkhon the Blight") != 0) {
        printf("Error: Final boss battle setup incorrect.\n");
        return -1;
    }

    printf("\nArkhon the Blight focuses his malevolent gaze upon you! The air crackles with dark energy!\n");
    // Boss HP is managed internally and indicated descriptively as per GameFlow.
    // Let's track boss HP as 'stages' or 'hits needed' for simplicity with descriptive feedback.
    int bossHitsTaken = 0;
    int bossHitsNeededToDefeat = 3; // Example: Blight needs to be successfully hit 3 times.

    // Spells and their incantations for the final battle
    const char* offensiveSpells[] = {"Lightning_Arc", "Flame_Spark"}; // Player's options
    const char* offensiveIncantations[] = {"Fulmen Arcana", "Ignis Minor"};
    int offensiveSpellDamage[] = {2, 1}; // Hits to boss health

    const char* defensiveSpells[] = {"Shield_of_Dawn"}; // Player's options for defense
    const char* defensiveIncantations[] = {"Aegis Lucis"};

    // Boss attacks (descriptive, tied to player needing to cast a defensive spell)
    const char* bossAttackDescriptions[] = {
        "Arkhon the Blight hurls a torrent of shadowflame at you!",
        "Tendrils of pure darkness erupt from the ground, lashing towards you!",
        "The Blight unleashes a deafening roar that tears at your very soul!"
    };
    int numBossAttackTypes = sizeof(bossAttackDescriptions) / sizeof(bossAttackDescriptions[0]);

    while (player->currentHp > 0 && bossHitsTaken < bossHitsNeededToDefeat) {
        printf("\nYour HP: %d/%d | Arkhon the Blight's remaining power: ", player->currentHp, player->maxHp);
        if (bossHitsTaken == 0) printf("Overwhelming\n");
        else if (bossHitsTaken == 1) printf("Waning, but still immense\n");
        else if (bossHitsTaken == 2) printf("Severely weakened, on the verge of collapse!\n");

        // Player Attack Phase
        printf("\n-- Your Attack Phase --\n");
        printf("Choose your offensive spell: (1) Lightning_Arc (Fulmen Arcana) (2) Flame_Spark (Ignis Minor)\n");
        printf("Enter spell number (or type 'status'): ");
        char choice[50];
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0;

        int spellChoice = atoi(choice) -1; // 0 for Lightning, 1 for Flame Spark

        if (strcasecmp(choice, "status") == 0) {
            displayPlayerStatus(player);
            continue; // Doesn't consume turn
        }

        if (spellChoice == 0 || spellChoice == 1) {
            if (hasSpell(player, offensiveSpells[spellChoice])) {
                printf("Prepare to cast %s!\n", offensiveSpells[spellChoice]);
                if (castSpellTimed(offensiveIncantations[spellChoice], 10)) { // 10 sec timer
                    printf("Your %s strikes Arkhon the Blight! He recoils in pain!\n", offensiveSpells[spellChoice]);
                    bossHitsTaken += offensiveSpellDamage[spellChoice];
                    if (bossHitsTaken >= bossHitsNeededToDefeat) break; // Boss defeated
                } else {
                    printf("Your spell fails! The Blight laughs mockingly.\n");
                }
            } else {
                printf("You don't know %s! You hesitate, and the Blight seizes the opening!\n", offensiveSpells[spellChoice]);
                // No direct damage, but Blight gets to attack immediately.
            }
        } else {
            printf("Invalid choice. You fumble, and the Blight prepares his assault!\n");
            // No direct damage, but Blight gets to attack.
        }
        
        if (player->currentHp <= 0) break; // Check if player died from a previous turn's delayed effect (if any)
        if (bossHitsTaken >= bossHitsNeededToDefeat) break;

        // Boss Attack Phase
        printf("\n-- Arkhon the Blight's Attack Phase --\n");
        int randomAttack = rand() % numBossAttackTypes;
        printf("%s\n", bossAttackDescriptions[randomAttack]);
        printf("You must defend! Recite the incantation for Shield_of_Dawn (Aegis Lucis):\n");

        if (hasSpell(player, "Shield_of_Dawn")) {
            if (castSpellTimed(defensiveIncantations[0], 8)) { // 8 sec timer for defense
                printf("Your Shield of Dawn flares, deflecting the Blight's assault! You stand firm!\n");
            } else {
                printf("Your shield falters! The Blight's attack slams into you!\n");
                takeDamage(player, 25 + (rand() % 11)); // Boss damage: 25-35
            }
        } else {
            printf("You don't know Shield_of_Dawn! You are defenseless against the assault!\n");
            takeDamage(player, 30 + (rand() % 16)); // Higher damage if no shield spell known: 30-45
        }

        if (player->currentHp <= 0) break; // Check if player died
    }

    if (bossHitsTaken >= bossHitsNeededToDefeat) {
        return 1; // Player wins
    } else if (player->currentHp <= 0) {
        return -1; // Player loses
    } else {
        // Should not be reached if loop logic is correct
        printf("DEBUG: Final battle ended inconclusively.\n");
        return -1; 
    }
} 