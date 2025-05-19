#include "player.h"
#include <stdio.h>
#include <string.h>

void initializePlayer(Player *player, const char *startLocation) {
    if (player == NULL) return;

    player->maxHp = 100;
    player->currentHp = player->maxHp;
    player->learnedSpellCount = 0;
    player->inventoryItemCount = 0;
    player->progressionFlags = 0;
    strncpy(player->currentLocation, startLocation, MAX_ITEM_NAME_LENGTH -1);
    player->currentLocation[MAX_ITEM_NAME_LENGTH - 1] = '\0';

    /* Initialize empty spellbook and inventory */
    for (int i = 0; i < MAX_SPELLS; ++i) {
        player->spellbook[i].name[0] = '\0';
    }
    for (int i = 0; i < MAX_INVENTORY_ITEMS; ++i) {
        player->inventory[i].name[0] = '\0';
    }
}

void displayPlayerStatus(const Player *player) {
    if (player == NULL) return;

    printf("\n--- Player Status ---\n");
    printf("HP: %d/%d\n", player->currentHp, player->maxHp);
    printf("Location: %s\n", player->currentLocation);

    printf("Spells Known (%d):\n", player->learnedSpellCount);
    if (player->learnedSpellCount == 0) {
        printf("  None\n");
    } else {
        for (int i = 0; i < player->learnedSpellCount; ++i) {
            printf("  - %s\n", player->spellbook[i].name);
        }
    }

    printf("Inventory (%d):\n", player->inventoryItemCount);
    if (player->inventoryItemCount == 0) {
        printf("  Empty\n");
    } else {
        for (int i = 0; i < player->inventoryItemCount; ++i) {
            printf("  - %s\n", player->inventory[i].name);
        }
    }
    printf("---------------------\n");
}

int hasSpell(const Player *player, const char *spellName) {
    if (player == NULL || spellName == NULL) return 0;

    for (int i = 0; i < player->learnedSpellCount; ++i) {
        if (strcmp(player->spellbook[i].name, spellName) == 0) {
            return 1; /* Player knows the spell */
        }
    }
    return 0; /* Player does not know the spell */
}

void learnSpell(Player *player, const char *spellName) {
    if (player == NULL || spellName == NULL) return;

    if (player->learnedSpellCount < MAX_SPELLS) {
        if (!hasSpell(player, spellName)) { /* Check if spell already learned */
            strncpy(player->spellbook[player->learnedSpellCount].name, spellName, MAX_SPELL_NAME_LENGTH - 1);
            player->spellbook[player->learnedSpellCount].name[MAX_SPELL_NAME_LENGTH - 1] = '\0';
            player->learnedSpellCount++;
            printf("You have learned the spell: %s!\n", spellName);
        } else {
            printf("You already know the spell: %s.\n", spellName);
        }
    } else {
        printf("Your mind cannot hold more spells! (Max spells reached)\n");
    }
}

int hasItem(const Player *player, const char *itemName) {
    if (player == NULL || itemName == NULL) return 0;

    for (int i = 0; i < player->inventoryItemCount; ++i) {
        if (strcmp(player->inventory[i].name, itemName) == 0) {
            return 1; /* Player has the item */
        }
    }
    return 0; /* Player does not have the item */
}

void addItemToInventory(Player *player, const char *itemName) {
    if (player == NULL || itemName == NULL) return;

    if (player->inventoryItemCount < MAX_INVENTORY_ITEMS) {
        if (!hasItem(player, itemName)) { /* Check if item already in inventory */
            strncpy(player->inventory[player->inventoryItemCount].name, itemName, MAX_ITEM_NAME_LENGTH - 1);
            player->inventory[player->inventoryItemCount].name[MAX_ITEM_NAME_LENGTH - 1] = '\0';
            player->inventoryItemCount++;
            printf("%s has been added to your inventory.\n", itemName);
        } else {
            printf("You already have %s.\n", itemName);
        }
    } else {
        printf("Your inventory is full! Cannot add %s.\n", itemName);
    }
}

void takeDamage(Player *player, int damageAmount) {
    if (player == NULL) return;

    player->currentHp -= damageAmount;
    printf("You take %d damage.\n", damageAmount);
    if (player->currentHp <= 0) {
        player->currentHp = 0;
        printf("Your vision fades... You have fallen.\n");
        /* Game over logic handled in the main game loop */
    }
}

void healPlayer(Player *player, int healAmount) {
    if (player == NULL) return;

    player->currentHp += healAmount;
    if (player->currentHp > player->maxHp) {
        player->currentHp = player->maxHp;
    }
    printf("You heal for %d HP. Current HP: %d/%d\n", healAmount, player->currentHp, player->maxHp);
}

int checkProgressionFlag(const Player *player, unsigned int flag) {
    if (player == NULL) return 0;
    return (player->progressionFlags & flag) == flag;
}

void setProgressionFlag(Player *player, unsigned int flag) {
    if (player == NULL) return;
    player->progressionFlags |= flag;
} 