#ifndef map_h
#define map_h

#include <stdint.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

#include "utility.h"

// IF YOU WANT TO INCREASE YOUR MAP SIZE OR ENEMY SIZE MAKE SURE TO INCREASE DEFINE VARIABLE BELOW IF IT EXCEED THE MAXIMUM
#define MAX_MAP_ROW 100
#define MAX_MAP_COL 100
#define MAX_ENEMY_SPAWN 100

extern int appleStat;
extern int bananaStat;

/*
    Map Tiles type
    Hint : Some type might need a background, to configure it try to modify "get_map_offset(Map * map)"
*/
typedef enum _BLOCK_TYPE {
    FLOOR,
    WALL,
    DOOR_CLOSE,
    DOOR_OPEN,
    BUTTON,
    HOLE,
    COIN,
    TROPHY,
    APPLE,
    BANANA,
    KEY,
    CHEST,
    CHEST_COIN,
    NOTHING
} BLOCK_TYPE;

typedef enum _COIN_STATUS {
    APPEAR,
    DISAPPEARING,
    DISAPPEAR
} COIN_STATUS;

typedef enum {
    APPLE_APPEAR,
    APPLE_DISAPPEARING,
    APPLE_DISAPPEAR
} APPLE_STATUS;

typedef enum {
    BANANA_APPEAR,
    BANANA_DISAPPEARING,
    BANANA_DISAPPEAR
} BANANA_STATUS; 

typedef enum doorStatus {
    OPEN,
    OPENED,
    CLOSE
} DOOR_STATUS;

typedef enum chestS {
    CHEST_CLOSED,
    CHEST_OPENING,
    CHEST_OPENED,
} CHEST_STATUS;


typedef enum buttonStatus {
    UNPRESSED,
    PRESSING,
    PRESSED
} BUTTON_STATUS;

// Map Struct
typedef struct Map_{
    uint8_t ** map; // Map array
    Point ** offset_assets; // Assets
    
    int row, col;
    
    // Map assets
    ALLEGRO_BITMAP* assets;
    ALLEGRO_BITMAP* coin_assets;
    

    // Coin Properties
    ALLEGRO_SAMPLE* coin_audio;
    uint8_t coin_animation_tick;
    uint8_t dis_coin_animation_tick[MAX_MAP_ROW][MAX_MAP_COL];

    //Trophy Properties
    ALLEGRO_BITMAP* trophy_assets;
    uint8_t trophy_animation_tick;
    ALLEGRO_SAMPLE* trophy_audio;

    //Apple Propereties
    ALLEGRO_BITMAP* apple_assets;
    ALLEGRO_SAMPLE* apple_audio;
    uint8_t apple_animation_tick;
    uint8_t dis_apple_animation_tick[MAX_MAP_ROW][MAX_MAP_COL];

    //Banana Properties
    ALLEGRO_BITMAP* banana_assets;
    ALLEGRO_SAMPLE* banana_audio;
    uint8_t banana_animation_tick;
    uint8_t dis_banana_animation_tick[MAX_MAP_ROW][MAX_MAP_COL];

    ALLEGRO_BITMAP* key_assets;
    ALLEGRO_BITMAP* chest_assets;
    ALLEGRO_BITMAP* door_assets;
    ALLEGRO_BITMAP* button_assets;
    ALLEGRO_SAMPLE* key_audio;
    ALLEGRO_SAMPLE* chest_audio;
    ALLEGRO_SAMPLE* door_audio;
    ALLEGRO_SAMPLE* button_audio;
    uint8_t door_animation_tick;
    uint8_t button_animation_tick;
    uint8_t chest_animation_tick;

    //
    // 
    // Properties
    ALLEGRO_SAMPLE* lose_audio;

    // Spawn Coordinate
    Point Spawn;
    Point EnemySpawn[MAX_ENEMY_SPAWN];
    char EnemyCode[MAX_ENEMY_SPAWN];
    uint8_t EnemySpawnSize;

    COIN_STATUS coinStatus[MAX_MAP_ROW][MAX_MAP_COL];
    APPLE_STATUS appleStatus[MAX_MAP_ROW][MAX_MAP_COL];
    BANANA_STATUS bananaStatus[MAX_MAP_ROW][MAX_MAP_COL];
    DOOR_STATUS doorStatus;
    BUTTON_STATUS buttonStatus;
    CHEST_STATUS chestStatus;

} Map;

/*
    MAP FUNCTION
    Feel free to add more if you have some idea or interaction with the map
 */
Map create_map(char * path, uint8_t type); // Create a map based on given file path
void draw_map(Map * map, Point cam); // Draw the map
void update_map(Map * map, Point player_coord, int * total_coins); // Update map : you might want add some parameter here
void destroy_map(Map * map); // Destroy map

bool isWalkable(BLOCK_TYPE block);

#endif /* map_h */
