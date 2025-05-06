// Data file path
#define DATA_DIR "./data/"
#define DATA_FILENAME "data.bin"
#define DATA_PATH DATA_DIR DATA_FILENAME

// Graphical definitions
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
// Font definitions
#define FONT_PATH "./rsc/assets/fonts/PressStart2P-Regular.ttf"
#define MENU_BG_PATH "./rsc/img/menu-bg.png"  // TODO: render
#define FONT_SIZE_MAIN_MENU 24
#define FONT_SIZE_SUBTITLE 20
#define FONT_SIZE_INSTRUCTION 18
#define FONT_SIZE_PAUSE_MENU 12
// Sprite definitions
#define PLAYER_SPRITE "./rsc/img/sprites/player.png"
// Tile and room definitions
#define TILE_SIZE 64
#define MIN_ROOM_SIZE 5
#define MAX_ROOM_SIZE 15
#define WALL_TILE "./rsc/img/tiles/wall_tile.png"
#define FLOOR_TILE "./rsc/img/tiles/floor_tile.png"


// Main Menu button coordinates
#define MENU_TITLE_Y 100
#define MENU_LEFT_X 200
#define MENU_MIDDLE_X 540
#define MENU_RIGHT_X 900
#define MENU_MIDDLE_Y 400
#define MENU_BACK_X 100
#define MENU_BACK_Y 600
#define MENU_VOL_Y 350
#define MENU_LCOL_X 200
#define MENU_RCOL_X 800
#define MENU_INST_OFFSET 50
#define MENU_INST_SUBT 300


// Rooms per level limits
#define MIN_ROOM_COUNT 3
#define MAX_ROOM_COUNT 6

// Key Scancodes
#define ESC SDL_SCANCODE_ESCAPE
#define W_KEY SDL_SCANCODE_W
#define S_KEY SDL_SCANCODE_S
#define A_KEY SDL_SCANCODE_A
#define D_KEY SDL_SCANCODE_D

