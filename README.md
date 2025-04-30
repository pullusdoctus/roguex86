# roguex86

A simple roguelike written in x86 Assembly

## Design:

https://www.figma.com/proto/lQmrlr89a0NWFvnloMGxdz/Rogue?node-id=0-1&t=0UvDJrCifNqJSB3R-1

## Journey Map

https://www.canva.com/design/DAGiL2HVH34/TpPiZKPbWGLe-oINYRER5g/edit?utm_content=DAGiL2HVH34&utm_campaign=designshare&utm_medium=link2&utm_source=sharebutton

## Software Stack

- Linux
- nasm
- Any IDE
- git
- SDL
- gcc
- Figma
- Aseprite

## TODO

### Iter2
- [X] Implement menu (YY)
	- [ ] Click New Game > Loads (YY)
	- [X] Click Options > Goes to Options Screen (pd)
		- [X] Click Volume > Goes to Volume Screen (pd)
			- [ ] Volume Slider
			- [X] Click Back > Return to previous screen (YY)
		- [X] Click Difficulty > Goes to Difficulty Screen (YY)
			- [ ] Click a difficulty > Difficulty Changes
			- [X] Click Back > Return to previous screen
		- [X] Click Instructions > Goes to Instructions Screen (pd)
			- [X] Click Back > Return to previous screen
		- [X] Click Back > Return to previous screen (YY)
	- [X] Find menu music (pd)
- [ ] Design Menu Screens (Options, Volume, Difficulty, Instructions)
- [ ] Design loading screen (pd)
- [X] Design main character, 3 enemies (pd)
- [X] Start design of rooms (floor/wall tiles) (YY)

### YiYhuan
- [X] Start design of rooms (floor/wall tiles)
- [X] Design Options, Volume, Difficulty screen
- [X] Load Menu on startup
- [X] Back
- [ ] New Game
- [ ] Difficulty

### pullusdoctus
- [X] Design main character, 3 enemies
- [ ] Design Instructions screen
- [ ] Design loading screen
- [X] Find menu music
- [X] Options
- [ ] Volume
- [X] Instructions

### Future
- [ ] Design controls: WASD/Arrows
- [ ] Design level generation
    - [ ] Design ~five rooms
    - [ ] Figure out how to connect rooms
    - [ ] Limit floor size (< 10 floors, < 5 rooms)
    - [ ] Figure out how to place stairs/boss
