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

- [ ] Finish asset design: Loading Screen, Defeat Screen, Victory Screen
- [ ] Design level generation
    - [ ] Figure out how to connect rooms
    - [ ] Limit floor size (< 10 floors, < 5 rooms)
    - [ ] Figure out how to place stairs/boss
- [ ] Combat
    - [ ] attack
    - [ ] defend
    - [ ] Character dies, combat ends
    - [ ] Player dies, game ends
