# The Sundering of Arkhon

Wake without your memories, learn spells, and uncover the source of a spreading blight in a terminal fantasy adventure written in C.

The game welcomes you as **Arkhon's Awakening**. Built for a programming class, it combines exploration, NPC conversations, inventory, puzzles, and turn-based combat through typed commands.

```text
Welcome to Arkhon's Awakening!
------------------------------------

You awaken in a dimly lit chamber, your head throbbing.
You have no memory of who you are or how you got here.

~ Dungeon_of_Awakening ~
You see: Grizzik
Exits: Crossroads

Location: Dungeon_of_Awakening | HP: 100/100
What would you like to do next? (type 'help' for commands)
>
```

*Excerpt from an actual run; introductory description shortened for readability.*

## Play

You need a C compiler and Make. From the repository root:

```sh
make -B
./arkhons_awakening
```

`make -B` rebuilds from source instead of reusing the prebuilt executable and object files currently tracked in the repository. Those artifacts are macOS ARM64; rebuilding produces a binary for your own environment. The source uses POSIX `strings.h` functions, so use a Unix-like environment such as macOS, Linux, or WSL.

Try this opening sequence:

```text
talk Grizzik
get Mage_Staff
read lore
explore Crossroads
```

Type location, character, spell, and item names as shown by the game, including underscores.

## Commands

| Command | Action |
| --- | --- |
| `help` | Show the available commands |
| `look around` | Examine your current location |
| `explore <location_name>` | Move to an adjacent known location |
| `talk <character_name>` | Speak with an NPC |
| `get <item_name>` / `use <item_name>` | Pick up or use an item |
| `inventory` / `status` | Inspect items, health, and learned spells |
| `learn <spell_name>` / `cast <spell_name>` | Learn or cast a spell where the context allows it |
| `read lore` | Read the location's lore |
| `examine symbols` / `meditate` | Investigate context-specific symbols |
| `quit` | Exit the game |

Combat presents its own choices. The help menu also exposes `test combat <enemy_name>` for development testing.

## Explore the source

| File | Responsibility |
| --- | --- |
| [`main.c`](main.c) | Opening narrative and command loop |
| [`world.c`](world.c), [`world.h`](world.h) | Locations, NPCs, exploration, and story interactions |
| [`player.c`](player.c), [`player.h`](player.h) | Player state, inventory, and spells |
| [`combat.c`](combat.c), [`combat.h`](combat.h) | Combat encounters and choices |
| [`Makefile`](Makefile) | C11 build with compiler warnings enabled |
| [`GameFlow`](GameFlow), [`GameDesingDocument`](GameDesingDocument) | Original flow and design notes |

The source was compiled with `-Wall -Wextra -std=c11` and checked through a help-and-quit run on macOS. This is a coursework project; that smoke check is not a full playthrough of every story branch.
