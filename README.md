# roguex86

A simple roguelike written in x86 Assembly and C++

## User Manual

### Compilation

The current process of compilation only supports 64-bit Linux distributions. Note that gcc, g++, nasm, and SDL2 are needed. If you don't have them, install them from your distribution's package manager.

SDL2 modules needed:

- SDL2 TTF
- SDL2 Image

For ease of compilation, a Makefile is provided. Simply run the `make` command to compile.

Other commands:

- `make run`: compile (if needed) and run the game.
- `make clean`: remove files built during compilation.

### Running the game

Run the executable file generated in the `bin/` directory. Alternatively, compile and run in one go by executing `make run` in a terminal.

## Design:

For more details on how the game is designed, visit the [design directory](./design/)

## Software Stack

- Operating System: 64-bit Linux
- Compilers: nasm and gcc/g++
- Any IDE
- git
- Graphic Library: SDL2
- Figma
- Pixel Art Tool: Aseprite

## TODO

### Priorities

- [ ] Pause menu
    - [ ] When in game, ESC opens pause menu
    - [ ] From pause menu, can access inventory, see player stats, exit game
    - [ ] Pause menu is a little window that is drawn on top of the game, in a corner of the screen
- [ ] Inventory & Potions
    - [ ] Player has an inventory where they store Objects
    - [ ] Potions are a type of Object
    - [ ] When used (in game/in combat), Potions heal 20HP
- [ ] End Screens
    - [ ] Finish asset design: Loading Screen, Defeat Screen, Victory Screen
    - [ ] Loading Screen: when first entering the game, display "Welcome to the dungeon... Good luck getting out! (Press Enter to start...)"; when going to the next floor, display "Floor floor_number" for a while (~5 seconds)
    - [ ] Defeat Screen: display "You lost, too bad! Do you wish to try again? (Press Enter to retry, or ESC to go back to the main menu)"
    - [ ] Victory Screen: display "Lucky you, you managed to escape! Do you wish to try again? (Press Enter to retry, or ESC to go back to the main menu)"

### Nice to Have
- [ ] Combat Messages
- [ ] Music
