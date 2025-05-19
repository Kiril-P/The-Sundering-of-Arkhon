#ifndef WORLD_H
#define WORLD_H

#include "player.h"

#define MAX_LOCATION_NAME_LENGTH 50
#define MAX_NPC_NAME_LENGTH 50
#define MAX_DESCRIPTION_LENGTH 512
#define MAX_EXITS 5

typedef struct Location {
    char name[MAX_LOCATION_NAME_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    char exits[MAX_EXITS][MAX_LOCATION_NAME_LENGTH];
    int numExits;
} Location;

typedef struct NPC {
    char name[MAX_NPC_NAME_LENGTH];
    char dialogue[MAX_DESCRIPTION_LENGTH];
    char currentLocation[MAX_LOCATION_NAME_LENGTH];
} NPC;

/* Function Prototypes */
/* Handle player world exploration */
void processExplore(Player *player, const char *locationName);

/* Handle NPC interactions */
void processTalk(Player *player, const char *npcName);

/* Get location data from location name */
const Location* getLocationData(const char *locationName);

/* Get NPC data from NPC name */
const NPC* getNPCData(const char *npcName);

/* Initialize world data - locations and NPCs */
void initializeWorld();

#endif /* WORLD_H */ 