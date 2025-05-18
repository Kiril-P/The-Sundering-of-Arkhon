#include "world.h"
#include "player.h" // For Player struct and progression flags
#include "combat.h" // For Enemy struct, getEnemyByName, startCombat
#include <stdio.h>
#include <string.h>
#include <stdlib.h> // For NULL

#define MAX_LOCATIONS 15 // Increased for more locations
#define MAX_NPCS 5

// Global array to hold all locations in the game
static Location gameLocations[MAX_LOCATIONS];
static int numGameLocations = 0;

// Global array for NPCs
static NPC gameNPCs[MAX_NPCS];
static int numGameNPCs = 0;

// Helper to add an exit to a location
void addExit(Location *loc, const char *exitName) {
    if (loc && loc->numExits < MAX_EXITS) {
        strncpy(loc->exits[loc->numExits], exitName, MAX_LOCATION_NAME_LENGTH - 1);
        loc->exits[loc->numExits][MAX_LOCATION_NAME_LENGTH - 1] = '\0';
        loc->numExits++;
    }
}

// Initialize all predefined locations
void initializeLocations() {
    Location loc;

    // Dungeon of Awakening
    memset(&loc, 0, sizeof(Location)); // Clear struct before use
    strncpy(loc.name, "Dungeon_of_Awakening", MAX_LOCATION_NAME_LENGTH - 1);
    strncpy(loc.description, "You are in the Dungeon of Awakening. The air is stale and heavy with the scent of dust and ages past. Ancient inscriptions cover the walls.", MAX_DESCRIPTION_LENGTH - 1);
    addExit(&loc, "Crossroads");
    gameLocations[numGameLocations++] = loc;

    // Crossroads
    memset(&loc, 0, sizeof(Location));
    strncpy(loc.name, "Crossroads", MAX_LOCATION_NAME_LENGTH - 1);
    strncpy(loc.description, "You stand at a mountain crossroads, a biting wind whipping around you. Two paths diverge: one upwards to a menacing Evil_Castle, the other downwards to the Village_of_Eldermoor.", MAX_DESCRIPTION_LENGTH - 1);
    addExit(&loc, "Dungeon_of_Awakening");
    addExit(&loc, "Evil_Castle"); // Path exists, but might be blocked
    addExit(&loc, "Village_of_Eldermoor");
    gameLocations[numGameLocations++] = loc;

    // Village of Eldermoor
    memset(&loc, 0, sizeof(Location));
    strncpy(loc.name, "Village_of_Eldermoor", MAX_LOCATION_NAME_LENGTH - 1);
    strncpy(loc.description, "You have arrived at the Village of Eldermoor. It seems quiet, nestled in a valley. Smoke curls from a few chimneys. Notable locations are the Library_of_Elders, a Mystical_Shop, and the entrance to Direfang_Dungeon.", MAX_DESCRIPTION_LENGTH - 1);
    addExit(&loc, "Crossroads");
    addExit(&loc, "Library_of_Elders");
    addExit(&loc, "Mystical_Shop");
    addExit(&loc, "Direfang_Dungeon");
    addExit(&loc, "Whispering_Grove"); // As per GameFlow, accessible immediately
    addExit(&loc, "Caverns_of_Echoes"); // As per GameFlow, accessible immediately
    gameLocations[numGameLocations++] = loc;

    // Library of Elders
    memset(&loc, 0, sizeof(Location));
    strncpy(loc.name, "Library_of_Elders", MAX_LOCATION_NAME_LENGTH - 1);
    strncpy(loc.description, "You enter the Library of Elders. Ancient tomes line the walls, and the air smells of old parchment and knowledge.", MAX_DESCRIPTION_LENGTH - 1);
    addExit(&loc, "Village_of_Eldermoor");
    gameLocations[numGameLocations++] = loc;

    // Mystical Shop (Placeholder)
    memset(&loc, 0, sizeof(Location));
    strncpy(loc.name, "Mystical_Shop", MAX_LOCATION_NAME_LENGTH - 1);
    strncpy(loc.description, "The Mystical Shop is cluttered with strange artifacts and bubbling potions. The air hums with a subtle energy.", MAX_DESCRIPTION_LENGTH - 1);
    addExit(&loc, "Village_of_Eldermoor");
    gameLocations[numGameLocations++] = loc;

    // Direfang Dungeon (Entrance - Placeholder)
    memset(&loc, 0, sizeof(Location));
    strncpy(loc.name, "Direfang_Dungeon", MAX_LOCATION_NAME_LENGTH - 1);
    strncpy(loc.description, "The entrance to Direfang Dungeon gapes before you, a dark maw promising danger and perhaps reward.", MAX_DESCRIPTION_LENGTH - 1);
    addExit(&loc, "Village_of_Eldermoor");
    // TODO: Add internal rooms for Direfang Dungeon
    gameLocations[numGameLocations++] = loc;

    // Whispering Grove (Entrance - Placeholder)
    memset(&loc, 0, sizeof(Location));
    strncpy(loc.name, "Whispering_Grove", MAX_LOCATION_NAME_LENGTH - 1);
    strncpy(loc.description, "You step into the Whispering Grove. Eerie whispers seem to emanate from the rustling leaves of the ancient trees.", MAX_DESCRIPTION_LENGTH - 1);
    addExit(&loc, "Village_of_Eldermoor");
    gameLocations[numGameLocations++] = loc;

    // Caverns of Echoes (Entrance - Placeholder)
    memset(&loc, 0, sizeof(Location));
    strncpy(loc.name, "Caverns_of_Echoes", MAX_LOCATION_NAME_LENGTH - 1);
    strncpy(loc.description, "The Caverns of Echoes descend into darkness. Strange sounds echo from within.", MAX_DESCRIPTION_LENGTH - 1);
    addExit(&loc, "Village_of_Eldermoor");
    gameLocations[numGameLocations++] = loc;

    // Evil Castle (Exterior - Placeholder)
    memset(&loc, 0, sizeof(Location));
    strncpy(loc.name, "Evil_Castle", MAX_LOCATION_NAME_LENGTH - 1);
    // Update description based on forcefield status dynamically in processExplore
    // strncpy(loc.description, "The Evil Castle looms ominously. A shimmering forcefield currently blocks your path.", MAX_DESCRIPTION_LENGTH - 1);
    addExit(&loc, "Crossroads");
    gameLocations[numGameLocations++] = loc;
}

// Initialize all predefined NPCs
void initializeNPCs() {
    NPC npc;

    // Grizzik
    memset(&npc, 0, sizeof(NPC));
    strncpy(npc.name, "Grizzik", MAX_NPC_NAME_LENGTH - 1);
    strncpy(npc.currentLocation, "Dungeon_of_Awakening", MAX_LOCATION_NAME_LENGTH - 1);
    strncpy(npc.dialogue, "Grizzik: 'Great Arkhon! Ready to explore? Or perhaps you have questions for Grizzik?'", MAX_DESCRIPTION_LENGTH - 1);
    gameNPCs[numGameNPCs++] = npc;

    // Fiona the Librarian
    memset(&npc, 0, sizeof(NPC));
    strncpy(npc.name, "Fiona", MAX_NPC_NAME_LENGTH - 1);
    strncpy(npc.currentLocation, "Library_of_Elders", MAX_LOCATION_NAME_LENGTH - 1);
    strncpy(npc.dialogue, "Fiona: 'Welcome to the Library of Elders. Seek you knowledge, or perhaps to learn a spell?'", MAX_DESCRIPTION_LENGTH - 1);
    gameNPCs[numGameNPCs++] = npc;

    // Elder Maris (Placeholder)
    memset(&npc, 0, sizeof(NPC));
    strncpy(npc.name, "Elder_Maris", MAX_NPC_NAME_LENGTH - 1);
    strncpy(npc.currentLocation, "Village_of_Eldermoor", MAX_LOCATION_NAME_LENGTH - 1);
    strncpy(npc.dialogue, "Elder Maris: 'Welcome to Eldermoor, traveler. Our village has seen many strange things. Perhaps you are another?'", MAX_DESCRIPTION_LENGTH - 1);
    gameNPCs[numGameNPCs++] = npc;
}

void initializeWorld() {
    initializeLocations();
    initializeNPCs();
    printf("World initialized with %d locations and %d NPCs.\n", numGameLocations, numGameNPCs); // Debug message
}

const Location* getLocationData(const char *locationName) {
    for (int i = 0; i < numGameLocations; ++i) {
        if (strcasecmp(gameLocations[i].name, locationName) == 0) { // Case-insensitive comparison for robustness
            return &gameLocations[i];
        }
    }
    return NULL; // Location not found
}

const NPC* getNPCData(const char *npcName) {
    for (int i = 0; i < numGameNPCs; ++i) {
        if (strcasecmp(gameNPCs[i].name, npcName) == 0) {
            return &gameNPCs[i];
        }
    }
    return NULL;
}

void processExplore(Player *player, const char *locationNameInput) {
    const Location* currentLocData = getLocationData(player->currentLocation);
    int canMove = 0;
    int justWonGrovePuzzle = 0;

    if (!currentLocData) {
        printf("Error: Your current location data is missing! Returning to a known safe spot.\n");
        strncpy(player->currentLocation, "Dungeon_of_Awakening", MAX_LOCATION_NAME_LENGTH -1);
        player->currentLocation[MAX_LOCATION_NAME_LENGTH - 1] = '\0';
        currentLocData = getLocationData(player->currentLocation);
        if(!currentLocData) {
             printf("Critical error: Cannot find Dungeon_of_Awakening. Game cannot continue.\n");
             player->currentHp = 0; // End game
             return;
        }
    }

    for (int i = 0; i < currentLocData->numExits; ++i) {
        if (strcasecmp(currentLocData->exits[i], locationNameInput) == 0) {
            canMove = 1;
            break;
        }
    }

    if (strcasecmp(locationNameInput, "Evil_Castle") == 0 && 
        !checkProgressionFlag(player, FLAG_LEARNED_SHADOWS_BANE)) {
        printf("A shimmering forcefield blocks your path to the Evil_Castle. It repels you with a strong magical force.\n");
        return;
    }
    
    if (strcasecmp(locationNameInput, "Direfang_Dungeon") == 0) {
        if (!hasSpell(player, "Flame_Spark") || !hasSpell(player, "Shield_of_Dawn")) {
            printf("You step into the ominous entrance of Direfang Dungeon without the recommended spells...\n");
            printf("Almost immediately, overwhelming shadowy figures leap from the darkness, their forms too terrifying to comprehend!\n");
            printf("You are instantly overcome. Your vision fades to black...\n");
            player->currentHp = 0; 
            return; 
        } else {
            printf("You cautiously enter Direfang Dungeon, spells at the ready...\n");
            Enemy direfangSpider;
            if (getEnemyByName("Direfang_Spider", &direfangSpider)) {
                int combatResult = startCombat(player, &direfangSpider);
                if (combatResult == 1) { 
                    printf("You defeated the Direfang Spider! The immediate area seems clear.\n");
                    if (!checkProgressionFlag(player, FLAG_LEARNED_LIGHTNING_ARC)) { // Only award once
                        learnSpell(player, "Lightning_Arc"); 
                        setProgressionFlag(player, FLAG_LEARNED_LIGHTNING_ARC);
                        printf("Among the spider's remains, you find a scorched scroll. As you touch it, knowledge of 'Lightning_Arc' (Incantation: Fulmen Arcana) floods your mind!\n");
                        printf("Additional lore snippet: The spiders of Direfang were once mere cave dwellers, twisted by a dark influence seeping from the mountain's peak...\n");
                    }
                } else if (combatResult == 0) { 
                    printf("You managed to flee back to the entrance of Direfang Dungeon.\n");
                    return; 
                } else { 
                    return; 
                }
            } else {
                printf("Error: Could not load Direfang_Spider for combat.\n");
            }
            // If combat occurred, description of Direfang Dungeon itself will be handled when player is IN it.
            // For now, winning the fight means they are at the entrance, having cleared it.
        }
    }

    // Whispering Grove Logic
    if (strcasecmp(locationNameInput, "Whispering_Grove") == 0 && strcasecmp(player->currentLocation, "Whispering_Grove") != 0) {
        // Player is attempting to ENTER the grove
        printf("You push past thorny vines into the eerie Whispering_Grove.\n");
        printf("The air is thick with mist, and unsettling whispers seem to emanate from the ancient, gnarled trees.\n");

        // Puzzle: Riddle of the Whispering Trees
        // This puzzle is encountered upon first *entering* the main area of the grove from outside.
        // We need a flag to ensure this puzzle only happens once, or if specific sub-areas are implemented.
        // For now, let's use a new flag: FLAG_GROVE_PUZZLE_ATTEMPTED
        if (!checkProgressionFlag(player, FLAG_GROVE_PUZZLE_ATTEMPTED)) {
            printf("A low voice, seemingly from the trees themselves, echoes: \n");
            printf("'I have roots that no one sees, am taller than the trees. Upward I reach, but never grow. What am I?'\n");
            printf("Your answer (one word): ");
            char answer[50];
            fgets(answer, sizeof(answer), stdin);
            answer[strcspn(answer, "\n")] = 0;
            setProgressionFlag(player, FLAG_GROVE_PUZZLE_ATTEMPTED);

            if (strcasecmp(answer, "mountain") == 0) {
                printf("The whispers soften, a path clearing slightly. 'Wise... you may pass this trial.'\n");
                justWonGrovePuzzle = 1;
            } else {
                printf("A chilling laughter echoes. 'Foolish mortal!' Thorny vines lash out, and shadows deepen!\n");
                takeDamage(player, 15); // Penalty for wrong answer
                if (player->currentHp <= 0) return; // Player might die from penalty
                printf("The grove feels more oppressive now.\n");
            }
        }

        // Encounter: Whispering Wraith (if puzzle passed or if it's not the first entry with the puzzle)
        // For simplicity, the wraith appears after the puzzle attempt, or if puzzle already done.
        printf("As you venture deeper, a spectral figure, a Whispering_Wraith, materializes from the mist!\n");
        Enemy groveWraith;
        if (getEnemyByName("Whispering_Wraith", &groveWraith)) {
            int combatResult = startCombat(player, &groveWraith);
            if (combatResult == 1) {
                printf("The Whispering Wraith dissipates with a final, mournful sigh.\n");
                if (!checkProgressionFlag(player, FLAG_LEARNED_NATURES_EMBRACE)) { // Only award once
                    learnSpell(player, "Natures_Embrace");
                    setProgressionFlag(player, FLAG_LEARNED_NATURES_EMBRACE);
                    printf("A soft, green glow emanates from where the wraith vanished. You feel a soothing energy and learn 'Nature_s_Embrace' (Incantation: Terra Sanatio)!\n");
                    printf("Vision: You see a fleeting image of Arkhon, younger, tending to a wounded ancient tree, a look of sorrow on his face... 'Even in strength, one must nurture life,' a distant voice echoes.\n");
                }
            } else if (combatResult == 0) {
                printf("You fled from the Whispering Wraith, escaping the heart of the Grove for now.\n");
                // Player retreats from the deeper part of the grove, effectively remaining at grove entrance or returning to Village.
                // For simplicity, assume they remain at Whispering_Grove entrance.
                // Do not proceed to fully describe the grove if fled.
                return; 
            } else { // Player lost
                return;
            }
        } else {
            printf("Error: Could not load Whispering_Wraith for combat.\n");
        }
        if (player->currentHp <= 0) return;
    }

    // Caverns of Echoes Logic - Part 1: Entry and Troll Encounter
    if (strcasecmp(locationNameInput, "Caverns_of_Echoes") == 0 && 
        strcasecmp(player->currentLocation, "Caverns_of_Echoes") != 0 && 
        !checkProgressionFlag(player, FLAG_LEARNED_SHADOWS_BANE)) { // Only full sequence if spell not learned
        
        printf("You descend into the Caverns_of_Echoes. A chilling draft blows from the depths, carrying faint, mournful sounds.\n");
        printf("The air is heavy, and the darkness seems to press in from all sides. This place feels ancient and dangerous.\n");

        // Troll Encounter first
        printf("The path ahead is treacherous, with loose stones and deep chasms hidden in shadow.\n");
        printf("You hear a low growl. A massive Cave_Troll, disturbed by your presence, lumbers into view, brandishing a crude stone club!\n");
        Enemy caveTroll;
        if (getEnemyByName("Cave_Troll", &caveTroll)) {
            int combatResult = startCombat(player, &caveTroll);
            if (combatResult == 1) {
                printf("The Cave_Troll crashes to the ground, defeated. The immediate passage is clear.\n");
                setProgressionFlag(player, FLAG_CAVERNS_TROLL_DEFEATED); // New flag: Mark troll as defeated
                // Puzzle and reward will follow in the next part of the logic for this location if troll is beaten.
            } else if (combatResult == 0) { // Fled from Troll
                printf("You managed to flee from the Cave_Troll, scrambling back towards the entrance of the Caverns.\n");
                return; // Player does not enter Caverns, stays in previous location
            } else { // Lost to Troll (combatResult == -1)
                // Death message handled in startCombat/takeDamage. Main loop will catch player.currentHp <= 0.
                return; // Player died, explore stops
            }
        } else {
            printf("Error: Could not load Cave_Troll for combat.\n");
            return; // Don't proceed with location change if critical component fails
        }
        if (player->currentHp <= 0) return; // Double check after combat sequence
    }

    // Random encounter at Crossroads
    if (strcasecmp(locationNameInput, "Crossroads") == 0 && !checkProgressionFlag(player, FLAG_CROSSROADS_AMBUSH_DONE)) {
        printf("As you approach the crossroads, a rugged Goblin Scout leaps from behind a rock!\n");
        Enemy goblinScout;
        if (getEnemyByName("Goblin_Scout", &goblinScout)) {
            int combatResult = startCombat(player, &goblinScout);
            setProgressionFlag(player, FLAG_CROSSROADS_AMBUSH_DONE); // Ambush only happens once.
            if (combatResult == -1) return; // Player died
            if (combatResult == 0) {
                 printf("You fled from the Goblin Scout. The crossroads seem quiet now.\n");
                 // If player flees, they might end up in the location they were coming from, or still at crossroads.
                 // For simplicity, let's assume they still arrive at crossroads after fleeing.
            }
            if (combatResult == 1) {
                printf("You defeated the Goblin Scout! The path to the crossroads is clear.\n");
            }
        } else {
            printf("Error: Could not load Goblin_Scout for combat.\n");
        }
        if (player->currentHp <= 0) return; // If combat resulted in death
    }

    // Evil Castle Entry and Final Boss Trigger
    if (strcasecmp(locationNameInput, "Evil_Castle") == 0 && 
        strcasecmp(player->currentLocation, "Evil_Castle") != 0 && // Prevents re-trigger if already inside (though no sub-rooms yet)
        checkProgressionFlag(player, FLAG_LEARNED_SHADOWS_BANE)) {

        printf("You step into the Evil_Castle. The air is thick with dread and ancient power. This is the heart of the Blight.\n");
        printf("A towering figure cloaked in shadow stands before a twisted throne. It turns slowly.\n");
        printf("Arkhon the Blight: 'So, the other half finally arrives. Come to reclaim what was lost, or to be lost yourself?'\n");
        // Placeholder for more dialogue leading to combat
        printf("Arkhon the Blight: 'No matter. Your journey ends here!'\n");

        Enemy arkhonTheBlight;
        if (getEnemyByName("Arkhon_The_Blight", &arkhonTheBlight)) {
            printf("\n--- The Final Confrontation Begins! ---\n");
            int finalBattleResult = startFinalBossBattle(player, &arkhonTheBlight); // Use the new function

            if (finalBattleResult == 1) { // Player wins
                printf("Arkhon the Blight shrieks as his form unravels, light piercing his shadowy essence!\n");
                printf("Blight: 'This... is not... the end! I will return! I ALWAYS return!' His form dissipates into nothingness.\n");
                setProgressionFlag(player, FLAG_DEFEATED_BLIGHT_STANDARD);

                // Check for True Ending conditions
                // Simplified: Check if all major spells are learned as a proxy for lore discovery
                if (hasSpell(player, "Flame_Spark") && hasSpell(player, "Shield_of_Dawn") && 
                    hasSpell(player, "Lightning_Arc") && hasSpell(player, "Natures_Embrace") && 
                    hasSpell(player, "Shadows_Bane")) {
                    
                    printf("\nBut wait... a new sensation. The chamber grows still. A faint, pure light emanates from where the Blight vanished.\n");
                    printf("A gentle voice, your own yet resonant with power, whispers in your mind: 'The cycle can be broken. The sundered soul can be made whole.'\n");
                    printf("The final incantation comes to you, a spell of ultimate reunification: Anima Unificare\n");
                    printf("This is it! Your one chance to end the curse forever!\n");
                    
                    if (castSpellTimed("Anima Unificare", 15)) { // castSpellTimed is in combat.c
                        printf("\nTRUE ENDING: You recite 'Anima Unificare'. A blinding light fills the chamber!\n");
                        printf("The two halves of Arkhon's soul - Benevolent and Blight - merge into one complete being of immense wisdom and power, neither light nor dark but a perfect balance.\n");
                        printf("The curse is broken. The world is safe. You, Arkhon, are finally whole and at peace.\n");
                        printf("Congratulations! You have achieved the True Ending!\n");
                        player->currentHp = 0; // End game on a high note
                        return;
                    } else {
                        printf("The final incantation fails! The moment passes. The Blight's chilling promise of return hangs heavy in the air.\n");
                        printf("STANDARD ENDING: Arkhon the Blight is defeated, but his dark essence is merely banished, destined to return. The cycle continues, but for now, the land knows a temporary peace. Your quest is over... for now.\n");
                         player->currentHp = 0; // End game
                         return;
                    }
                } else {
                    printf("STANDARD ENDING: Arkhon the Blight is defeated, but his dark essence is merely banished, destined to return. The cycle continues, but for now, the land knows a temporary peace. Your quest is over... for now.\n");
                    player->currentHp = 0; // End game
                    return;
                }
            } else { // Player lost or fled (fleeing Blight is impossible in current startCombat)
                printf("Arkhon the Blight stands victorious. Darkness consumes all...\n");
                // Player HP should be 0 if lost, main loop handles game over message.
                return;
            }
        } else {
            printf("CRITICAL ERROR: Could not load Arkhon_The_Blight for final battle!\n");
            return;
        }
        // After battle (win/loss), the game should end. No further exploration of Evil_Castle here.
        return; // Explicitly return to prevent falling through to location description if battle ends game.
    }

    // If player is already in Evil_Castle (e.g. if sub-rooms were added later and they are moving within)
    // or if they are exploring it after Blight is defeated (not possible with current flow that ends game)
    if (strcasecmp(locationNameInput, "Evil_Castle") == 0 && checkProgressionFlag(player, FLAG_DEFEATED_BLIGHT_STANDARD)){
        printf("The Evil Castle is eerily silent now, the Blight's presence gone. Only echoes of the final battle remain.\n");
        // Game should have ended, but this is a fallback description.
    }

    if (!canMove && strcasecmp(player->currentLocation, locationNameInput) != 0) {
        printf("You can't seem to go to '%s' from here. Try looking at available exits.\n", locationNameInput);
        return;
    }

    const Location* newLocData = getLocationData(locationNameInput);

    if (newLocData) {
        strncpy(player->currentLocation, newLocData->name, MAX_LOCATION_NAME_LENGTH - 1);
        player->currentLocation[MAX_LOCATION_NAME_LENGTH - 1] = '\0';
        
        printf("\n~ %s ~\n", newLocData->name);
        // Dynamic description for Evil Castle based on forcefield
        if (strcasecmp(newLocData->name, "Evil_Castle") == 0) {
            if (checkProgressionFlag(player, FLAG_LEARNED_SHADOWS_BANE)) {
                printf("The Evil Castle looms ominously. The way is clear, the forcefield dispelled by your Shadow's Bane spell!\n");
            } else {
                printf("The Evil Castle looms ominously. A shimmering forcefield blocks your path.\n");
            }
        } else {
            printf("%s\n", newLocData->description); // Standard description
        }

        int npcFoundInLocation = 0;
        for(int i = 0; i < numGameNPCs; ++i) {
            if(strcasecmp(gameNPCs[i].currentLocation, newLocData->name) == 0) {
                if (!npcFoundInLocation) { printf("You see: "); npcFoundInLocation = 1; }
                printf("%s  ", gameNPCs[i].name);
            }
        }
        if (npcFoundInLocation) printf("\n");

        if (newLocData->numExits > 0) {
            printf("Exits: ");
            for (int i = 0; i < newLocData->numExits; ++i) {
                printf("%s  ", newLocData->exits[i]);
            }
            printf("\n");
        }

        if (strcasecmp(newLocData->name, "Dungeon_of_Awakening") == 0) {
            if (checkProgressionFlag(player, FLAG_GRIZZIK_MET) && !hasItem(player, "Mage_Staff")) {
                printf("Grizzik points to the staff. 'Don't forget your Mage_Staff, Arkhon!' (type 'get Mage_Staff')\n");
            }
             printf("You can 'read lore' from the inscriptions here or 'explore Crossroads'.\n");
        } else if (strcasecmp(newLocData->name, "Crossroads") == 0) {
            if (checkProgressionFlag(player, FLAG_LEARNED_SHADOWS_BANE)) {
                 printf("The forcefield around the Evil_Castle has vanished! The path is clear!\n");
            } else {
                printf("Grizzik (if with you): 'That forcefield on the Evil_Castle... still strong.'\n");
            }
        } else if (strcasecmp(newLocData->name, "Library_of_Elders") == 0) {
            const NPC* fiona = getNPCData("Fiona");
            if (fiona) {
                printf("Fiona, the librarian, looks up. '%s'\n", fiona->dialogue);
                if (!hasSpell(player, "Flame_Spark")) {
                    printf("Fiona offers, 'I can teach you Flame_Spark. Just say \'learn Flame_Spark\'.'\n");
                }
                if (!hasSpell(player, "Shield_of_Dawn")) {
                    printf("Fiona adds, 'Shield_of_Dawn is also available. Say \'learn Shield_of_Dawn\'.'\n");
                }
                printf("You can also try to 'read lore' here.\n");
            }
        } else if (strcasecmp(newLocData->name, "Whispering_Grove") == 0) {
            // This is printed when player is *in* the grove (after entry events)
            printf("The whispers in the grove are incessant. Paths twist and turn through the ancient trees.\n");
            if (checkProgressionFlag(player, FLAG_LEARNED_NATURES_EMBRACE)) {
                printf("You feel a faint connection to the healing energies you discovered here.\n");
            } else if (justWonGrovePuzzle) {
                 printf("The path revealed by solving the riddle seems to lead deeper, but an eerie presence still lingers.\n");
            } else if (checkProgressionFlag(player, FLAG_GROVE_PUZZLE_ATTEMPTED)){
                printf("The grove feels oppressive and confusing, the shadows clinging to you after failing the trees' riddle.\n");
            }
            // TODO: Add more specific interactions or sub-areas for Whispering Grove.
        } else if (strcasecmp(newLocData->name, "Caverns_of_Echoes") == 0) {
            // This is printed when player is *in* the caverns (after entry events)
            // Part 2 of Caverns of Echoes logic will go here (puzzle & reward if troll defeated)
            if (checkProgressionFlag(player, FLAG_LEARNED_SHADOWS_BANE)) {
                printf("The chamber where you faced the Troll and solved the riddle of echoes is quiet now, the symbols on the wall dormant.\n");
                printf("You recall the profound lore revealed here about your true nature and Arkhon the Blight.\n");
            } else if (checkProgressionFlag(player, FLAG_CAVERNS_TROLL_DEFEATED) && !checkProgressionFlag(player, FLAG_CAVERNS_PUZZLE_SOLVED)) {
                // Troll is defeated, but puzzle not yet solved/triggered
                printf("Beyond the fallen troll, the cavern opens slightly. Strange symbols pulse faintly on a far wall, awaiting your understanding...\n");
                printf("(You might need to 'examine symbols' or a similar command here to trigger the puzzle - this will be implemented next)\n");
            } else if (checkProgressionFlag(player, FLAG_CAVERNS_TROLL_DEFEATED) && checkProgressionFlag(player, FLAG_CAVERNS_PUZZLE_SOLVED)){
                // Should ideally be caught by FLAG_LEARNED_SHADOWS_BANE check first
                 printf("The echoes of the puzzle you solved still resonate. The ultimate truth granted by it is already known to you.\n"); 
            } else {
                 printf("The deep darkness of the Caverns seems to watch you. The Cave Troll's growl still echoes in your memory if you haven't faced it, or its defeat has opened a new mystery.\n");
            }
        }
         // Check if Grizzik should follow the player and provide commentary
        if (checkProgressionFlag(player, FLAG_GRIZZIK_MET) && strcasecmp(newLocData->name, "Dungeon_of_Awakening") != 0) {
            // Grizzik follows you out of the dungeon.
            // For simplicity, let's assume Grizzik is always with player after meeting him, for now.
            // We can refine this later to make him stay in certain areas or leave.
            // His comments can be integrated into location descriptions or specific triggers.
        }

    } else if (canMove) { // It was a valid exit, but location data not found (should not happen with current setup)
        printf("Error: The path to '%s' seems to exist, but the area itself is undefined. This is a bug.\n", locationNameInput);
    }
    // If !canMove and not same location, message already printed.
}

void processTalk(Player *player, const char *npcNameInput) {
    const NPC* npc = getNPCData(npcNameInput);

    if (!npc) {
        printf("There is no one called '%s' here, or they are not talkative.\n", npcNameInput);
        return;
    }

    if (strcasecmp(player->currentLocation, npc->currentLocation) != 0) {
        printf("%s is not in your current location (%s).\n", npc->name, player->currentLocation);
        return;
    }

    printf("\n~ Talking to %s ~\n", npc->name);
    printf("%s\n", npc->dialogue);

    // Specific NPC interactions
    if (strcasecmp(npc->name, "Grizzik") == 0) {
        if (strcasecmp(player->currentLocation, "Dungeon_of_Awakening") == 0 && !hasItem(player, "Mage_Staff")) {
            printf("Grizzik: 'Don't forget your staff, Arkhon! It's right over there!'\n");
        }
        // Add more Grizzik context-specific dialogue
    } else if (strcasecmp(npc->name, "Fiona") == 0) {
        if (strcasecmp(player->currentLocation, "Library_of_Elders") == 0) {
            if (!hasSpell(player, "Flame_Spark") || !hasSpell(player, "Shield_of_Dawn")) {
                printf("Fiona: 'If you wish to learn a spell, just tell me to \'learn <spell_name>\'.'\n");
            } else {
                printf("Fiona: 'Have you had a chance to read some of the lore here? It might prove insightful.'\n");
            }
        }
    } else if (strcasecmp(npc->name, "Elder_Maris") == 0) {
        printf("Elder Maris: 'Arkhon... a name whispered in legends. They say Arkhon's soul was split. One benevolent, one a blight. If you are who I think you are, a difficult path lies ahead.'\n");
        // Set a lore flag if Maris reveals this
        // setProgressionFlag(player, FLAG_LORE_ARKHON_SPLIT_MARIS);
    }
    // Add other NPC specific dialogues
} 