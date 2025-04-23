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
- [ ] Implement menu (YY)
	- [ ] Click New Game > Loads (YY)
	- [ ] Click Options > Goes to Options Screen (pd)
		- [ ] Click Volume > Goes to Volume Screen (pd)
			- [ ] Volume Slider
			- [ ] Click Back > Return to previous screen (YY)
		- [ ] Click Difficulty > Goes to Difficulty Screen (YY)
			- [ ] Click a difficulty > Difficulty Changes
			- [ ] Click Back > Return to previous screen
		- [ ] Click Instructions > Goes to Instructions Screen (pd)
			- [ ] Click Back > Return to previous screen
		- [ ] Click Back > Return to previous screen (YY)
	- [ ] Find menu music (pd)
- [ ] Design Menu Screens (Options, Volume, Difficulty, Instructions)
- [ ] Design loading screen (pd)
- [ ] Design main character, 3 enemies (pd)
- [ ] Start design of rooms (floor/wall tiles) (YY)

### YiYhuan
- [ ] Start design of rooms (floor/wall tiles)
- [ ] Design Options, Volume, Difficulty screen
- [ ] Load Menu on startup
- [ ] Back
- [ ] New Game
- [ ] Difficulty

### pullusdoctus
- [ ] Design main character, 3 enemies
- [ ] Design Instructions screen
- [ ] Design loading screen
- [ ] Find menu music
- [ ] Options
- [ ] Volume
- [ ] Instructions

### Future
- [ ] Design controls: WASD/Arrows
- [ ] Design level generation
    - [ ] Design ~five rooms
    - [ ] Figure out how to connect rooms
    - [ ] Limit floor size (< 10 floors, < 5 rooms)
    - [ ] Figure out how to place stairs/boss