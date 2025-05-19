#include "combat.h"
#include "player.h" // For Player, Spell, Item, MAX_SPELL_NAME_LENGTH, hasSpell, takeDamage etc.
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // For rand(), srand()
#include <time.h>   // For time() in castSpell
#include <strings.h> // For strcasecmp

/* Core spell casting mechanic with time limit */
int castSpellTimed(const char *correctSpell, int timeLimitSeconds) {
    char input[100];
    time_t start_time, end_time;
    time(&start_time);

    printf("Recite the incantation for %s (%d seconds limit):\n> ", correctSpell, timeLimitSeconds);
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0; /* Remove newline */

    time(&end_time);
    if (difftime(end_time, start_time) > timeLimitSeconds) {
        printf("Too slow! The magic fizzles.\n");
        return 0; /* Too slow */
    }

    /* Case-insensitive comparison for player convenience */
    if (strcasecmp(input, correctSpell) == 0) {
        return 1; /* Correct spell cast */
    }

    printf("Incorrect incantation! The spell fails.\n");
    return 0; /* Incorrect spell */
}

/* Advanced multi-stage spell casting for complex spells */
int castMultiStageSpellTimed(const char* spellNameForDisplay, const char* incantations[], int numStages, int timeLimitPerStage) {
    char input[100];
    time_t start_time, end_time;

    for (int i = 0; i < numStages; ++i) {
        time(&start_time);
        printf("Recite %s - Stage %d/%d: %s (%d seconds limit):\n> ", 
               spellNameForDisplay, i + 1, numStages, incantations[i], timeLimitPerStage);
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0; /* Remove newline */
        time(&end_time);

        if (difftime(end_time, start_time) > timeLimitPerStage) {
            printf("Too slow for stage %d! The magic destabilizes and the spell fails.\n", i + 1);
            return 0; /* Spell fails */
        }

        if (strcasecmp(input, incantations[i]) != 0) {
            printf("Incorrect incantation for stage %d! The spell unravels.\n", i + 1);
            return 0; /* Spell fails */
        }
        printf("Stage %d/%d successful!\n", i + 1, numStages);
    }

    return 1; /* All stages completed successfully */
}

/* Create an enemy instance based on name */
int getEnemyByName(const char *enemyName, Enemy *targetEnemy) {
    if (!targetEnemy || !enemyName) return 0;
    memset(targetEnemy, 0, sizeof(Enemy)); /* Clear the struct first */

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
        targetEnemy->maxHp = 60;
        targetEnemy->currentHp = targetEnemy->maxHp;
        targetEnemy->baseAttack = 15;
        targetEnemy->baseDefense = 8;
        strncpy(targetEnemy->description, "A spectral figure from the Whispering Grove, its touch chills to the bone.", sizeof(targetEnemy->description) - 1);
        return 1;
    } else if (strcasecmp(enemyName, "Cave_Troll") == 0) {
        strncpy(targetEnemy->name, "Cave Troll", MAX_ENEMY_NAME_LENGTH - 1);
        targetEnemy->maxHp = 100;
        targetEnemy->currentHp = targetEnemy->maxHp;
        targetEnemy->baseAttack = 20;
        targetEnemy->baseDefense = 10;
        strncpy(targetEnemy->description, "A hulking Cave Troll from the Caverns of Echoes, brandishing a massive club.", sizeof(targetEnemy->description) - 1);
        return 1;
    } else if (strcasecmp(enemyName, "Arkhon_The_Blight") == 0) {
        strncpy(targetEnemy->name, "Arkhon the Blight", MAX_ENEMY_NAME_LENGTH - 1);
        targetEnemy->maxHp = 250;
        targetEnemy->currentHp = targetEnemy->maxHp;
        targetEnemy->baseAttack = 25;
        targetEnemy->baseDefense = 15;
        strncpy(targetEnemy->description, "Your dark reflection, Arkhon the Blight, radiates immense power and malice.", sizeof(targetEnemy->description) - 1);
        return 1;
    }

    return 0; /* Enemy type not found */
}

/* Main combat function */
int startCombat(Player *player, Enemy *enemy) {
    if (!player || !enemy) return -1; /* Error case */

    printf("\n--- Encounter! --- \n%s\n", enemy->description);
    printf("You face %s (HP: %d)!\n", enemy->name, enemy->currentHp);

    int playerShieldTurns = 0; /* Duration for Shield of Dawn effect */

    char combatChoice[100];
    while(player->currentHp > 0 && enemy->currentHp > 0) {
        printf("\nYour HP: %d/%d | %s's HP: %d/%d", player->currentHp, player->maxHp, enemy->name, enemy->currentHp, enemy->maxHp);
        if (playerShieldTurns > 0) printf(" (Shielded)");
        printf("\nCombat options: attack, spell <n>, spells, status, flee\n> ");
        fgets(combatChoice, sizeof(combatChoice), stdin);
        combatChoice[strcspn(combatChoice, "\n")] = 0;

        if (strcasecmp(combatChoice, "attack") == 0) {
            /* Simple physical attack */
            int damageDealt = 10 + (rand() % 6); /* Base player attack: 10-15 damage */
            damageDealt -= enemy->baseDefense;
            if (damageDealt < 0) damageDealt = 0;
            printf("You strike %s with your staff for %d damage!\n", enemy->name, damageDealt);
            enemy->currentHp -= damageDealt;
        } else if (strncasecmp(combatChoice, "spell ", 6) == 0) {
            char spellToUse[MAX_SPELL_NAME_LENGTH];
            sscanf(combatChoice + 6, "%49s", spellToUse);
            if (hasSpell(player, spellToUse)) {
                if (strcasecmp(spellToUse, "Flame_Spark") == 0) {
                    if (castSpellTimed("Ignis Minor", 10)) {
                        int spellDamage = 20 + (rand() % 11); /* Flame Spark: 20-30 damage */
                        printf("A brilliant spark of flame erupts from your staff, scorching %s for %d damage!\n", enemy->name, spellDamage);
                        enemy->currentHp -= spellDamage;
                    } else {
                        printf("Your Flame Spark fizzles due to incorrect or slow incantation!\n");
                        continue; /* Try another action this turn */
                    }
                } else if (strcasecmp(spellToUse, "Shield_of_Dawn") == 0) {
                     if (castSpellTimed("Aegis Lucis", 8)) {
                        printf("A shimmering shield of light envelops you, bolstering your defense!\n");
                        playerShieldTurns = 2; /* Shield lasts for 2 enemy attacks */
                     } else {
                        printf("Your Shield of Dawn fails to materialize!\n");
                        continue; /* Try another action this turn */
                     }
                } else if (strcasecmp(spellToUse, "Lightning_Arc") == 0) {
                    const char* lightningIncantations[] = {"Fulmen Primus", "Arcus Maximus", "Tonitrus Impetus"};
                    if (castMultiStageSpellTimed("Lightning_Arc", lightningIncantations, 3, 7)) {
                        int spellDamage = 45 + (rand() % 21); /* Lightning Arc: 45-65 damage */
                        printf("A tremendous crackling arc of lightning, woven from three potent incantations, engulfs %s for %d damage!\n", enemy->name, spellDamage);
                        enemy->currentHp -= spellDamage;
                    } else {
                        printf("Your Lightning Arc spell failed to fully materialize!\n");
                        continue; /* Try another action this turn */
                    }
                } else if (strcasecmp(spellToUse, "Natures_Embrace") == 0) {
                    if (castSpellTimed("Terra Sanatio", 10)) {
                        int healAmount = 50 + (rand() % 21); /* Nature's Embrace: 50-70 HP heal */
                        healPlayer(player, healAmount);
                    } else {
                        printf("The healing energies of Nature's Embrace fail to coalesce!\n");
                        continue; /* Try another action this turn */
                    }
                }
                else {
                    printf("You can't use '%s' effectively in this combat right now, or it's not an offensive/defensive spell for this context.\n", spellToUse);
                }
            } else {
                printf("You don't know the spell '%s'.\n", spellToUse);
            }
        } else if (strcasecmp(combatChoice, "flee") == 0) {
            /* Boss prevents fleeing */
            if (strcasecmp(enemy->name, "Arkhon the Blight") == 0) {
                printf("Arkhon the Blight mocks your attempt: 'You cannot escape your own shadow!' Fleeing is impossible!\n");
            } else {
                printf("You attempt to flee...\n");
                if ((rand() % 100) < 75) { /* 75% chance to flee non-bosses */
                    printf("You successfully escape from %s!\n", enemy->name);
                    return 0; /* Player fled */
                } else {
                    printf("You couldn't find an opening to escape!\n");
                }
            }
        } else if (strcasecmp(combatChoice, "status") == 0) {
            displayPlayerStatus(player);
            continue; /* Does not consume a turn */
        } else if (strcasecmp(combatChoice, "spells") == 0) {
            printf("Known spells:\n");
            for (int i = 0; i < player->learnedSpellCount; ++i) {
                printf("  - %s\n", player->spellbook[i].name);
            }
            if (player->learnedSpellCount == 0) {
                printf("  You don't know any spells yet.\n");
            }
            continue; /* Does not consume a turn */
        } else {
            printf("Invalid combat command. Options: attack, spell <n>, spells, status, flee.\n");
            continue; /* Invalid command doesn't consume a turn */
        }

        if (enemy->currentHp <= 0) {
            printf("\n%s has been defeated!\n", enemy->name);
            if (strcasecmp(enemy->name, "Direfang Spider") == 0) {
                printf("The spider dissolves into dust, leaving behind a strange, pulsating ichor.\n");
            }
            return 1; /* Player won */
        }

        /* Enemy Attack Phase */
        if (enemy->currentHp > 0) {
            printf("\n%s prepares to attack!\n", enemy->name);
            int enemyDamage = enemy->baseAttack - (playerShieldTurns > 0 ? 5 : 0); /* Shield reduces damage */
            if (playerShieldTurns > 0) {
                printf("Your shield absorbs some of the blow!\n");
                playerShieldTurns--;
            }
            if (enemyDamage < 0) enemyDamage = 1; /* Minimum 1 damage if shielded heavily */
            
            printf("%s attacks you for %d damage!\n", enemy->name, enemyDamage);
            takeDamage(player, enemyDamage);

            if (player->currentHp <= 0) {
                return -1; /* Player lost */
            }
        }
    }

    if (player->currentHp <=0) return -1;
    if (enemy->currentHp <=0) return 1;
    return 0; /* Default to flee/stalemate if loop exits unexpectedly */
}

/* Final Boss Battle Implementation */
int startFinalBossBattle(Player *player, Enemy *boss) {
    if (!player || !boss || strcasecmp(boss->name, "Arkhon the Blight") != 0) {
        printf("Error: Final boss battle setup incorrect.\n");
        return -1;
    }

    printf("\nArkhon the Blight focuses his malevolent gaze upon you! The air crackles with dark energy!\n");
    
    int bossHitsTaken = 0;
    int bossHitsNeededToDefeat = 3; /* Boss needs to be hit 3 times to defeat */

    /* Battle configuration */
    const char* offensiveSpells[] = {"Lightning_Arc", "Flame_Spark"};
    const char* offensiveIncantations[] = {"Fulmen Arcana", "Ignis Minor"};
    int offensiveSpellDamage[] = {2, 1}; /* Damage values to boss health */

    const char* defensiveSpells[] = {"Shield_of_Dawn"};
    const char* defensiveIncantations[] = {"Aegis Lucis"};

    /* Boss attack descriptions */
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

        /* Player Attack Phase */
        printf("\n-- Your Attack Phase --\n");
        printf("Choose your offensive spell: (1) Lightning_Arc (Fulmen Arcana) (2) Flame_Spark (Ignis Minor)\n");
        printf("Enter spell number (or type 'spells' to list, 'status' for player status): ");
        char choice[50];
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0;

        if (strcasecmp(choice, "status") == 0) {
            displayPlayerStatus(player);
            continue; /* Re-prompt for attack */
        } else if (strcasecmp(choice, "spells") == 0) {
            printf("Known spells for offensive phase:\n");
            printf("  - Lightning_Arc (Incantation: Fulmen Arcana)\n");
            printf("  - Flame_Spark (Incantation: Ignis Minor)\n");
            continue; /* Re-prompt for attack */
        }

        int spellChoiceNum = atoi(choice);

        if (spellChoiceNum == 0 || spellChoiceNum == 1) {
            if (hasSpell(player, offensiveSpells[spellChoiceNum])) {
                printf("Prepare to cast %s!\n", offensiveSpells[spellChoiceNum]);
                if (castSpellTimed(offensiveIncantations[spellChoiceNum], 10)) {
                    printf("Your %s strikes Arkhon the Blight! He recoils in pain!\n", offensiveSpells[spellChoiceNum]);
                    bossHitsTaken += offensiveSpellDamage[spellChoiceNum];
                    if (bossHitsTaken >= bossHitsNeededToDefeat) break; /* Boss defeated */
                } else {
                    printf("Your spell fails! The Blight laughs mockingly.\n");
                }
            } else {
                printf("You don't know %s! You hesitate, and the Blight seizes the opening!\n", offensiveSpells[spellChoiceNum]);
            }
        } else {
            printf("Invalid choice. You fumble, and the Blight prepares his assault!\n");
        }
        
        if (player->currentHp <= 0) break;
        if (bossHitsTaken >= bossHitsNeededToDefeat) break;

        /* Boss Attack Phase */
        printf("\n-- Arkhon the Blight's Attack Phase --\n");
        int attackIndex = rand() % numBossAttackTypes;
        printf("%s\n", bossAttackDescriptions[attackIndex]);
        printf("Choose your defensive spell: (1) Shield_of_Dawn (Aegis Lucis)\n");
        printf("Enter spell number (or type 'spells' to list, 'status' for player status): ");
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0;

        if (strcasecmp(choice, "status") == 0) {
            displayPlayerStatus(player);
            continue; /* Re-prompt for defense */
        } else if (strcasecmp(choice, "spells") == 0) {
            printf("Known spells for defensive phase:\n");
            printf("  - Shield_of_Dawn (Incantation: Aegis Lucis)\n");
            continue; /* Re-prompt for defense */
        }

        spellChoiceNum = atoi(choice);

        if (spellChoiceNum == 0) {
            if (hasSpell(player, defensiveSpells[0])) {
                if (castSpellTimed(defensiveIncantations[0], 8)) {
                    printf("Your Shield of Dawn flares, deflecting the Blight's assault! You stand firm!\n");
                } else {
                    printf("Your shield falters! The Blight's attack slams into you!\n");
                    takeDamage(player, 25 + (rand() % 11)); /* Boss damage: 25-35 */
                }
            } else {
                printf("You don't know Shield_of_Dawn! You are defenseless against the assault!\n");
                takeDamage(player, 30 + (rand() % 16)); /* Higher damage if no shield: 30-45 */
            }
        } else {
            printf("Invalid choice. You fumble, and the Blight's attack hits you!\n");
            takeDamage(player, 25 + (rand() % 11)); /* Standard boss damage */
        }

        if (player->currentHp <= 0) break; /* Check if player died */
    }

    if (bossHitsTaken >= bossHitsNeededToDefeat) {
        return 1; /* Player wins */
    } else if (player->currentHp <= 0) {
        return -1; /* Player loses */
    } else {
        printf("DEBUG: Final battle ended inconclusively.\n");
        return -1; 
    }
} 