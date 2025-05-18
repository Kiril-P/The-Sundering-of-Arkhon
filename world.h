#ifndef WORLD_H
#define WORLD_H

#include "player.h" // Needs Player struct for interactions

#define MAX_LOCATION_NAME_LENGTH 50
#define MAX_NPC_NAME_LENGTH 50
#define MAX_DESCRIPTION_LENGTH 512
#define MAX_EXITS 5

typedef struct Location {
    char name[MAX_LOCATION_NAME_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    char exits[MAX_EXITS][MAX_LOCATION_NAME_LENGTH]; // Names of locations accessible from here
    int numExits;
    // We can add flags for puzzles, required items/spells to enter, etc.
    // e.g., int requiresShadowBane;
} Location;

typedef struct NPC {
    char name[MAX_NPC_NAME_LENGTH];
    char dialogue[MAX_DESCRIPTION_LENGTH]; // Basic dialogue, can be expanded
    char currentLocation[MAX_LOCATION_NAME_LENGTH]; // Where the NPC is
    // We can add flags for quests, information given, etc.
} NPC;

// Function Prototypes

// This will replace the exploreLocation in main.c
void processExplore(Player *player, const char *locationName);

// This will handle talking to NPCs
void processTalk(Player *player, const char *npcName);

// Utility to get location data (implementation will be in world.c)
const Location* getLocationData(const char *locationName);

// Utility to get NPC data (implementation will be in world.c)
const NPC* getNPCData(const char *npcName);

// Initialize all world data (locations, NPCs)
void initializeWorld();

#endif // WORLD_H 