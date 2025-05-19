#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <strings.h> /* For strcasecmp */

#include "player.h"
#include "world.h"
#include "combat.h"

/* Function prototypes */
void gameStartNarration(Player *player);

/* Main Game Loop */
int main() {
    srand(time(NULL));

    Player player;
    initializePlayer(&player, "Dungeon_of_Awakening");
    initializeWorld();

    printf("Welcome to Arkhon's Awakening!\n");
    printf("------------------------------------\n\n");

    gameStartNarration(&player);
    processExplore(&player, player.currentLocation);

    char choice[100];
    while (player.currentHp > 0) {
        printf("\nLocation: %s | HP: %d/%d\n", player.currentLocation, player.currentHp, player.maxHp);
        printf("What would you like to do next? (type 'help' for commands)\n> ");
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0; /* Remove newline character */

        if (strlen(choice) == 0) { /* Handle empty input */
            continue;
        }

        if (strcasecmp(choice, "quit") == 0) {
            printf("Exiting Arkhon's Awakening. Farewell!\n");
            break;
        } else if (strcasecmp(choice, "help") == 0) {
            printf("Commands:\n");
            printf("  explore <location_name> - Move to an adjacent, known location.\n");
            printf("  look around             - Examine your current surroundings again.\n");
            printf("  talk <character_name>   - Speak with an NPC.\n");
            printf("  cast <spell_name>       - Attempt to cast a known spell (context-dependent).\n");
            printf("  learn <spell_name>      - Learn a new spell if offered.\n");
            printf("  status                  - Check your player status, spells, and inventory.\n");
            printf("  inventory               - Check your items.\n");
            printf("  get <item_name>         - Pick up an item.\n");
            printf("  read lore               - Read lore available in the current location.\n");
            printf("  examine symbols         - Focus on peculiar symbols (context-dependent).\n");
            printf("  meditate                - Alias for 'examine symbols'.\n");
            printf("  test combat <enemy_name> - Initiate a test combat.\n");
            printf("  use <item_name>         - Use an item.\n");
            printf("  quit                    - Exit the game.\n");
        } else if (strncasecmp(choice, "explore ", 8) == 0) {
            char locationToExplore[MAX_LOCATION_NAME_LENGTH];
            if (sscanf(choice + 8, "%49s", locationToExplore) == 1) {
                 processExplore(&player, locationToExplore);
            } else {
                printf("Explore where? (e.g., explore Crossroads). Check available exits.\n");
            }
        } else if (strcasecmp(choice, "look around") == 0) {
            processExplore(&player, player.currentLocation);
        } else if (strncasecmp(choice, "talk ", 5) == 0) {
            char characterToTalk[MAX_NPC_NAME_LENGTH];
            if (sscanf(choice + 5, "%49s", characterToTalk) == 1) {
                processTalk(&player, characterToTalk);
            } else {
                printf("Talk to whom? (e.g., talk Grizzik)\n");
            }
        } else if (strncasecmp(choice, "cast ", 5) == 0) {
            char spellToCast[MAX_SPELL_NAME_LENGTH];
            if (sscanf(choice + 5, "%49s", spellToCast) == 1) {
                 printf("You attempt to cast %s...\n", spellToCast);
                 if(hasSpell(&player, spellToCast)){
                    printf("You feel the magical energies stir. The spell is ready, awaiting a purpose or target.\n");
                    
                    /* Handle special case - dispelling forcefield */
                    if (strcasecmp(player.currentLocation, "Crossroads") == 0 && 
                        strcasecmp(spellToCast, "Shadows_Bane") == 0 && 
                        hasSpell(&player, "Shadows_Bane") && 
                        !checkProgressionFlag(&player, FLAG_LEARNED_SHADOWS_BANE)) {
                        
                        printf("You channel the energies of Shadow's Bane towards the shimmering forcefield...\n");
                        printf("A dark pulse emanates from your staff, and the forcefield around the Evil Castle wavers violently before vanishing with a pop!\n");
                    }

                 } else {
                    printf("You don't know the spell '%s'.\n", spellToCast);
                 }
            } else {
                printf("Cast what spell? (e.g., cast Flame_Spark)\n");
            }
        } else if (strcasecmp(choice, "status") == 0) {
            displayPlayerStatus(&player);
        } else if (strcasecmp(choice, "inventory") == 0) {
            displayPlayerStatus(&player);
        } else if (strncasecmp(choice, "get ", 4) == 0) {
            char itemToGet[MAX_ITEM_NAME_LENGTH];
            if (sscanf(choice + 4, "%49s", itemToGet) == 1) {
                if (strcasecmp(player.currentLocation, "Dungeon_of_Awakening") == 0 && strcasecmp(itemToGet, "Mage_Staff") == 0) {
                    if (!hasItem(&player, "Mage_Staff")) {
                        addItemToInventory(&player, "Mage_Staff");
                        setProgressionFlag(&player, FLAG_STAFF_RETRIEVED);
                        printf("Grizzik: 'Excellent! With the Mage's Staff, your power will surely return!'\n");
                    } else {
                        printf("You already have the Mage's Staff.\n");
                    }
                } else if (strcasecmp(player.currentLocation, "Direfang_Dungeon") == 0 && strcasecmp(itemToGet, "Direfang_Pendant") == 0) {
                    if (checkProgressionFlag(&player, FLAG_DIREFANG_PENDANT_VISIBLE) && !checkProgressionFlag(&player, FLAG_DIREFANG_PENDANT_COLLECTED)) {
                        addItemToInventory(&player, "Direfang_Pendant");
                        setProgressionFlag(&player, FLAG_DIREFANG_PENDANT_COLLECTED);
                        printf("You pick up the Direfang_Pendant. It feels strangely warm to the touch and hums with a faint energy.\n");
                    } else if (checkProgressionFlag(&player, FLAG_DIREFANG_PENDANT_COLLECTED)) {
                        printf("You already have the Direfang_Pendant.\n");
                    } else {
                        printf("You don't see any Direfang_Pendant here right now.\n");
                    }
                } else {
                    printf("You can't seem to get '%s' here, or it doesn't exist.\n", itemToGet);
                }
            } else {
                printf("Get what? (e.g., get Mage_Staff)\n");
            }
        } else if (strncasecmp(choice, "learn ", 6) == 0) {
            char spellToLearn[MAX_SPELL_NAME_LENGTH];
            if (sscanf(choice + 6, "%49s", spellToLearn) == 1) {
                if (strcasecmp(player.currentLocation, "Library_of_Elders") == 0) {
                    const NPC* fiona = getNPCData("Fiona");
                    if (fiona && strcasecmp(fiona->currentLocation, player.currentLocation) == 0) {
                        if (strcasecmp(spellToLearn, "Flame_Spark") == 0) {
                            if (!hasSpell(&player, "Flame_Spark")) {
                                learnSpell(&player, "Flame_Spark");
                                setProgressionFlag(&player, FLAG_LEARNED_FLAME_SPARK);
                                printf("Fiona smiles. 'Flame_Spark is a humble but useful incantation. Incantation: Ignis Minor. Use it wisely.'\n");
                            } else {
                                printf("Fiona: 'You already know Flame_Spark.'\n");
                            }
                        } else if (strcasecmp(spellToLearn, "Shield_of_Dawn") == 0) {
                            if (!hasSpell(&player, "Shield_of_Dawn")) {
                                learnSpell(&player, "Shield_of_Dawn");
                                setProgressionFlag(&player, FLAG_LEARNED_SHIELD_OF_DAWN);
                                printf("Fiona nods. 'Shield_of_Dawn will serve you well. Incantation: Aegis Lucis.'\n");
                            } else {
                                printf("Fiona: 'You are already versed in the Shield_of_Dawn.'\n");
                            }
                        } else {
                            printf("Fiona cocks her head. 'I do not know of a spell called %s, or cannot teach it here.'\n", spellToLearn);
                        }
                    } else {
                         printf("Fiona doesn't seem to be here to teach you.\n");
                    }
                } else {
                    printf("You can't seem to learn spells here. Perhaps a library or a tutor is needed.\n");
                }
            } else {
                printf("Learn what spell? (e.g., learn Flame_Spark)\n");
            }
        } else if (strcasecmp(choice, "read lore") == 0) {
            if (strcasecmp(player.currentLocation, "Library_of_Elders") == 0) {
                printf("You spend some time perusing the ancient texts... 'On the night the mountain wept fire, Arkhon's ambition fractured the cosmos... Two essences, bound yet opposed...' (Sample Lore Excerpt)\n");
                printf("You also read about the 'Sundering of Arkhon', detailing a great conflict and a magical split.\n");
            } else if (strcasecmp(player.currentLocation, "Dungeon_of_Awakening") == 0) {
                 printf("Inscriptions: '...soul divided... blight contained... slumber eternal until the twin returns...' (Environmental Lore)\n");
            } else { printf("There is no particular lore to read here, or it is part of the location description.\n"); }
        } else if (strncasecmp(choice, "test combat ", 12) == 0) {
            char enemyToTest[MAX_ENEMY_NAME_LENGTH];
            if (sscanf(choice + 12, "%49s", enemyToTest) == 1) {
                Enemy combatant;
                if (getEnemyByName(enemyToTest, &combatant)) {
                    printf("DEBUG: Initiating test combat with %s.\n", combatant.name);
                    int combatResult = startCombat(&player, &combatant);
                    if (combatResult == 1) {
                        printf("Test combat won!\n");
                    } else if (combatResult == 0) {
                        printf("Escaped from test combat.\n");
                    } else {
                        printf("Test combat lost. Player HP: %d\n", player.currentHp);
                    }
                } else {
                    printf("DEBUG: Enemy type '%s' not found for test combat.\nAvailable: Direfang_Spider, Goblin_Scout, Whispering_Wraith, Cave_Troll, Arkhon_The_Blight\n", enemyToTest);
                }
            } else {
                printf("Test combat with which enemy? (e.g., test combat Goblin_Scout)\n");
            }
        } else if (strcasecmp(choice, "examine symbols") == 0 || strcasecmp(choice, "meditate") == 0) {
            if (strcasecmp(player.currentLocation, "Caverns_of_Echoes") == 0) {
                if (checkProgressionFlag(&player, FLAG_CAVERNS_TROLL_DEFEATED) && 
                    !checkProgressionFlag(&player, FLAG_LEARNED_SHADOWS_BANE)) {
                    
                    printf("You focus on the strange symbols pulsing faintly on the far wall, remnants of the troll's domain...\n");
                    printf("An ethereal voice whispers, echoing through the chamber: \n");
                    printf("'To walk the true path, one must understand the echo of their own nature...\n");
                    printf("  1. Embrace the consuming Blight within.\n");
                    printf("  2. Seek the fading Light of Benevolence.\n");
                    printf("  3. Acknowledge both Shadow and Light as one origin.\n");
                    printf("Choose your understanding (1, 2, or 3): ");
                    char puzzleChoice[10];
                    fgets(puzzleChoice, sizeof(puzzleChoice), stdin);
                    puzzleChoice[strcspn(puzzleChoice, "\n")] = 0;

                    if (strcmp(puzzleChoice, "3") == 0) {
                        printf("The symbols on the wall flare brightly, then resolve into a single, complex glyph.\n");
                        printf("Voice: 'Understanding dawns... The shadow and the light... two halves of a fractured whole... Your whole...'\n");
                        learnSpell(&player, "Shadows_Bane"); 
                        setProgressionFlag(&player, FLAG_LEARNED_SHADOWS_BANE); 
                        setProgressionFlag(&player, FLAG_CAVERNS_PUZZLE_SOLVED);
                        printf("The knowledge of 'Shadows_Bane' (Incantation: Umbra Mortis) sears into your mind! This spell can dispel dark barriers.\n");
                        printf("CRITICAL LORE: The voice continues, 'You are Arkhon, but not whole. The Blight you hunt is the other half of your sundered soul, sealed away long ago by your own hand, an act of desperation to save the world... and yourself. To truly end the cycle, the souls must be one.'\n");
                        processExplore(&player, player.currentLocation);
                    } else if (strcmp(puzzleChoice, "1") == 0 || strcmp(puzzleChoice, "2") == 0) {
                        printf("Voice: 'A partial truth... The path remains clouded.' The symbols fade slightly, offering no further enlightenment now.\n");
                        printf("You feel a wave of despair wash over you, a missed opportunity. (You take 10 psychic damage)\n");
                        takeDamage(&player, 10);
                    } else {
                        printf("Voice: 'Your understanding is muddled.' A tremor shakes the cavern, and rocks fall from the ceiling! (You take 20 damage)\n");
                        takeDamage(&player, 20);
                    }
                } else if (checkProgressionFlag(&player, FLAG_LEARNED_SHADOWS_BANE)) {
                    printf("You gaze at the symbols, but their secrets have already been revealed to you.\n");
                } else if (!checkProgressionFlag(&player, FLAG_CAVERNS_TROLL_DEFEATED)){
                    printf("You try to focus, but the menacing presence of the Cave Troll (or its recent memory) prevents deep concentration on any symbols here.\n");
                } else {
                    printf("There are no particular symbols to examine here, or they hold no meaning for you now.\n");
                }
            } else {
                printf("You find no such symbols to examine or reason to meditate here.\n");
            }
        } else if (strncasecmp(choice, "use ", 4) == 0) {
            char itemToUse[MAX_ITEM_NAME_LENGTH];
            if (sscanf(choice + 4, "%49s", itemToUse) == 1) {
                if (strcasecmp(itemToUse, "Minor_Healing_Potion") == 0) {
                    if (hasItem(&player, "Minor_Healing_Potion")) {
                        healPlayer(&player, 75);
                        printf("You drink the Minor_Healing_Potion. A warm energy spreads through you, mending some of your wounds. (HP +25)\n");
                    } else {
                        printf("You don't have a Minor_Healing_Potion to use.\n");
                    }
                } else {
                    printf("You can't use '%s' right now, or don't know how.\n", itemToUse);
                }
            } else {
                printf("Use what? (e.g., use Minor_Healing_Potion)\n");
            }
        } else {
            printf("Unknown command: '%s'. Type 'help' for a list of commands.\n", choice);
        }
    }

    return 0;
}

void gameStartNarration(Player *player) {
    printf("You awaken in a dimly lit chamber, your head throbbing. You have no memory of who you are or how you got here.\n");
    setProgressionFlag(player, FLAG_GRIZZIK_MET);
} 