#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <strings.h> // For strcasecmp

#include "player.h"
#include "world.h"
#include "combat.h" // Include combat header

// Enemy struct is now in combat.h
// Function prototypes for functions still in main.c
void gameStartNarration(Player *player);

// --- Main Game Loop ---
int main() {
    srand(time(NULL));

    Player player;
    initializePlayer(&player, "Dungeon_of_Awakening"); // Initial location from world.c logic
    initializeWorld(); // Initialize all locations and NPCs

    printf("Welcome to Arkhon's Awakening!\n");
    printf("------------------------------------\n\n");

    gameStartNarration(&player);
    processExplore(&player, player.currentLocation); // Describe the starting location

    char choice[100];
    while (player.currentHp > 0) {
        printf("\nLocation: %s | HP: %d/%d\n", player.currentLocation, player.currentHp, player.maxHp);
        printf("What would you like to do next? (type 'help' for commands)\n> ");
        fgets(choice, sizeof(choice), stdin);
        choice[strcspn(choice, "\n")] = 0; // Remove newline character

        if (strlen(choice) == 0) { // Handle empty input
            continue;
        }

        // Normalize to lowercase for command processing if desired, or use strcasecmp for specific commands
        // For simplicity, using strcasecmp for some known commands and strncmp for prefix commands.

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
            printf("  get <item_name>         - Pick up an item.\n");
            printf("  read lore               - Read lore available in the current location.\n");
            printf("  examine symbols         - Focus on peculiar symbols (context-dependent, e.g., Caverns_of_Echoes).\n");
            printf("  meditate                - Alias for 'examine symbols'.\n");
            printf("  test combat <enemy_name> - Initiate a test combat (e.g., test combat Goblin_Scout).\n");
            printf("  quit                    - Exit the game.\n");
        } else if (strncasecmp(choice, "explore ", 8) == 0) {
            char locationToExplore[MAX_LOCATION_NAME_LENGTH];
            if (sscanf(choice + 8, "%49s", locationToExplore) == 1) { // Skip "explore "
                 processExplore(&player, locationToExplore);
            } else {
                printf("Explore where? (e.g., explore Crossroads). Check available exits.\n");
            }
        } else if (strcasecmp(choice, "look around") == 0) {
            processExplore(&player, player.currentLocation); // Re-describe current location using world module
        } else if (strncasecmp(choice, "talk ", 5) == 0) {
            char characterToTalk[MAX_NPC_NAME_LENGTH];
            if (sscanf(choice + 5, "%49s", characterToTalk) == 1) { // Skip "talk "
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
                    // Actual spell effects will be tied to combat or specific interactions.
                    // For example, Shadow's Bane might be cast on the Evil_Castle forcefield if player is at Crossroads
                    if (strcasecmp(player.currentLocation, "Crossroads") == 0 && 
                        strcasecmp(spellToCast, "Shadows_Bane") == 0 && 
                        hasSpell(&player, "Shadows_Bane") && 
                        !checkProgressionFlag(&player, FLAG_LEARNED_SHADOWS_BANE)) { // FLAG_LEARNED_SHADOWS_BANE indicates it was already used here or learned status
                        // This is a bit of a hack. Ideally, Shadow's Bane has a specific target (the forcefield)
                        // For now, we assume casting it at the crossroads if you know it implies targeting the forcefield.
                        // The game documents state Shadow's Bane dispels the forcefield. It doesn't say *how* it's targeted.
                        printf("You channel the energies of Shadow's Bane towards the shimmering forcefield...\n");
                        // The GameFlow states: "Spell Shadow's Bane (Dispels forcefield around Evil Castle)"
                        // It is learned from Caverns of Echoes.
                        // We need a flag to say the forcefield is down. The FLAG_LEARNED_SHADOWS_BANE is for knowing the spell.
                        // Let's use a new flag or repurpose one carefully. The check in processExplore uses FLAG_LEARNED_SHADOWS_BANE.
                        // This implies learning it IS using it, or it auto-dispels. Let's stick to the existing flag for now.
                        // The visual change is handled by processExplore when re-entering Crossroads.
                        printf("A dark pulse emanates from your staff, and the forcefield around the Evil Castle wavers violently before vanishing with a pop!\n");
                        // setProgressionFlag(&player, FLAG_FORCEFIELD_DOWN); // A dedicated flag might be cleaner.
                        // For now, processExplore at Crossroads checks FLAG_LEARNED_SHADOWS_BANE to describe the state.
                    }

                 } else {
                    printf("You don't know the spell '%s'.\n", spellToCast);
                 }
            } else {
                printf("Cast what spell? (e.g., cast Flame_Spark)\n");
            }
        } else if (strcasecmp(choice, "status") == 0) {
            displayPlayerStatus(&player);
        } else if (strncasecmp(choice, "get ", 4) == 0) {
            char itemToGet[MAX_ITEM_NAME_LENGTH];
            if (sscanf(choice + 4, "%49s", itemToGet) == 1) { // Skip "get "
                if (strcasecmp(player.currentLocation, "Dungeon_of_Awakening") == 0 && strcasecmp(itemToGet, "Mage_Staff") == 0) {
                    if (!hasItem(&player, "Mage_Staff")) {
                        addItemToInventory(&player, "Mage_Staff");
                        setProgressionFlag(&player, FLAG_STAFF_RETRIEVED);
                        printf("Grizzik: 'Excellent! With the Mage's Staff, your power will surely return!'\n");
                    } else {
                        printf("You already have the Mage's Staff.\n");
                    }
                } else {
                    printf("You can't seem to get '%s' here, or it doesn't exist.\n", itemToGet);
                }
            } else {
                printf("Get what? (e.g., get Mage_Staff)\n");
            }
        } else if (strncasecmp(choice, "learn ", 6) == 0) {
            char spellToLearn[MAX_SPELL_NAME_LENGTH];
            if (sscanf(choice + 6, "%49s", spellToLearn) == 1) { // Skip "learn "
                if (strcasecmp(player.currentLocation, "Library_of_Elders") == 0) {
                    const NPC* fiona = getNPCData("Fiona"); // Check if Fiona is there
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
                        // After learning, re-describe the location to reflect the change
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
        } else {
            printf("Unknown command: '%s'. Type 'help' for a list of commands.\n", choice);
        }
    }

    if (player.currentHp <= 0 && strcmp(choice, "quit") != 0) {
        // Death message is handled in takeDamage in player.c, but ensure loop terminates
    }

    return 0;
}

void gameStartNarration(Player *player) {
    printf("You awaken in a dimly lit chamber, your head throbbing. You have no memory of who you are or how you got here.\n");
    // Grizzik's introduction is now handled by his presence in the Dungeon_of_Awakening (world.c) when explored.
    // And his dialogue via processTalk.
    setProgressionFlag(player, FLAG_GRIZZIK_MET); // Assume Grizzik is met immediately.
}

// Remove old castSpell and handleCombat definitions from here if they exist. 