#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#include "map.h"
#include "game_scene.h"
#include "player.h"

/*
    [OFFSET CALCULATOR FUNCTIONS]
      You might want to modify this functions if you are using different assets
      In short, this functions help us to find which asset to draw
 */
static void get_map_offset(Map * map);
static Point get_wall_offset_assets(Map * map, int i, int j);
static Point get_floor_offset_assets(Map * map, int i, int j);
static Point get_hole_offset_assets(Map * map, int i, int j);
static const int offset = 16;


static bool tile_collision(Point player, Point tile_coord);

int key_obtained;

int appleStat, bananaStat;
int applePrice, bananaPrice;

Map create_map(char * path, uint8_t type){
    Map map;

    memset(&map, 0, sizeof(Map));
    
    FILE * f = fopen(path, "r");
    if(!f){
        game_abort("Can't load map file : %s", path);
    }
    
    game_log("Using %s to load the map", path);
    
    fscanf(f, "%d %d", &map.row, &map.col);
    char ch;
    
    // Map Array
    map.map = (uint8_t **) malloc(map.row * sizeof(uint8_t *));
    for(int i=0; i<map.row; i++){
        map.map[i] = (uint8_t *) malloc(map.col * sizeof(uint8_t));
    }
    
    // Map Offset Resource Pack Array
    map.offset_assets = (Point **) malloc(map.row * sizeof(Point *));
    for(int i=0; i<map.row; i++){
        map.offset_assets[i] = (Point *) malloc(map.col * sizeof(Point));
    }

    int coin_counter = 0;
    
    
    // Scan the map to the array
    int door_counter = 0;
    for(int i = 0; i < map.row; i++){
        for(int j = 0; j < map.col; j++){
            fscanf(f, " %c", &ch);
            
            switch(ch)
            {
                case '#': // Case Wall
                    map.map[i][j] = WALL;
                    break;

                case '.': // Case Floor
                    map.map[i][j] = FLOOR;
                    break;

                case 'P': // Spawn Point
                    map.map[i][j] = FLOOR;
                    map.Spawn = (Point){i, j};
                    break;

                case 'S': // Slime Enemy
                    map.map[i][j] = FLOOR;
                    map.EnemyCode[map.EnemySpawnSize] = ch;
                    map.EnemySpawn[map.EnemySpawnSize++] = (Point){i, j};
                    break;

                case 'C': // Coins
                    map.map[i][j] = COIN;
                    map.coinStatus[i][j] = APPEAR;
                    coin_counter++;
                    break;

                case 'T': //Trophy
                    map.map[i][j] = TROPHY;
                    break;
                
                case 'A': //Apple
                    if (appleStat) {
                        map.map[i][j] = APPLE;
                        map.appleStatus[i][j] = APPLE_APPEAR;
                    }
                    else {
                        map.map[i][j] = FLOOR;
                    }
                    break;

                case 'B': //Banana
                    if (bananaStat) {
                        map.map[i][j] = BANANA;
                        map.bananaStatus[i][j] = BANANA_APPEAR;
                    }
                    else {
                        map.map[i][j] = FLOOR;
                    }
                    break;

                case 'D':
                    map.map[i][j] = DOOR_CLOSE;
                    map.doorStatus = CLOSE;
                    break;

                case 'V':
                    map.map[i][j] = BUTTON;
                    map.buttonStatus = UNPRESSED;
                    break;

                case 'G':
                    map.map[i][j] = CHEST;
                    map.chestStatus = CHEST_CLOSED;

                    break;

                case 'K':
                    if (key_obtained == 0) {
                        map.map[i][j] = KEY;
                        map.chestStatus = CHEST_CLOSED;
                    }
                    else {
                        map.map[i][j] = FLOOR;
                    }
                    break;

                case 'O':
                    coin_counter++;
                    map.map[i][j] = CHEST_COIN;
                    break;
                    

                case '_': // Nothing
                    map.map[i][j] = HOLE;
                    break;

                default: // If not determined, set it as black
                    map.map[i][j] = NOTHING;
                    break;
            }
        }
    }
    
    map.assets = al_load_bitmap("Assets/map_packets.png");
    if(!map.assets){
        game_abort("Can't load map assets");
    }

    map.coin_assets = al_load_bitmap("Assets/coins.png");
    if (!map.coin_assets) {
        game_abort("Can't load coin assets");
    }

    map.door_assets = al_load_bitmap("Assets/Door.png");
    map.button_assets = al_load_bitmap("Assets/Button.png");
    map.key_assets = al_load_bitmap("Assets/Key.png");
    map.chest_assets = al_load_bitmap("Assets/Chest_Locked.png");

    // load the offset for each tiles type
    get_map_offset(&map);

    map.trophy_assets = al_load_bitmap("Assets/trophy.png");
    if (!map.coin_assets) {
        game_abort("Can't load trophy assets");
    }

    map.apple_assets = al_load_bitmap("Assets/Apple.png");
    if (!map.apple_assets) {
        game_abort("Can't load apple assets");
    }

    map.banana_assets = al_load_bitmap("Assets/Banana.png");
    if (!map.banana_assets) {
        game_abort("Can't load banana assets");
    }

    map.trophy_audio = al_load_sample("Assets/audio/win.mp3");
    if (!map.trophy_audio) {
        game_abort("Can't load trophy audio");
    }

    map.lose_audio = al_load_sample("Assets/audio/lose.mp3");
    if (!map.lose_audio) {
        game_abort("Can't load lose audio");
    }

    map.coin_audio = al_load_sample("Assets/audio/coins.mp3");
    if(!map.coin_audio){
        game_abort("Can't load coin audio");
    }

    map.apple_audio = al_load_sample("Assets/audio/apple.mp3");
    if (!map.apple_audio) {
        game_abort("Can't load apple audio");
    }

    map.banana_audio = al_load_sample("Assets/audio/banana.mp3");
    if (!map.banana_audio) {
        game_abort("Can't load banana audio");
    }

    map.door_audio = al_load_sample("Assets/audio/door.mp3");
    map.button_audio = al_load_sample("Assets/audio/button.mp3");
    map.key_audio = al_load_sample("Assets/audio/key.mp3");
    map.chest_audio = al_load_sample("Assets/audio/chest.mp3");

    fclose(f);
    
    return map;
}

void draw_map(Map * map, Point cam){
    // Draw map based on the camera point coordinate
    al_clear_to_color(al_map_rgb(24, 20, 37));


    for(int i=0; i<map->row; i++){
        for(int j=0; j<map->col; j++){
            
            int dy = i * TILE_SIZE - cam.y; // destiny y axis
            int dx = j * TILE_SIZE - cam.x; // destiny x axis
            
            Point offset_asset = map->offset_assets[i][j];
            
            al_draw_scaled_bitmap(map->assets, // image
                                  offset_asset.x, offset_asset.y, 16, 16, // source x, source y, width, height
                                  dx, dy, TILE_SIZE, TILE_SIZE, // destiny x, destiny y, destiny width, destiny height
                                  0 // flag : set 0
                                  );

            switch(map->map[i][j]){
            case COIN: {
                    int offset_x = 0;
                    int offset_y = 0;
             
                    if (map->coinStatus[i][j] == APPEAR) {
                        offset_x = 16 * (map->coin_animation_tick / (64 / 8));
                        offset_y = 0;
                    }

                    if (map->coinStatus[i][j] == DISAPPEARING) {
                        map->dis_coin_animation_tick[i][j] =
                            (map->dis_coin_animation_tick[i][j] + 1) % 64;
                        offset_x = 16 * (map->dis_coin_animation_tick[i][j] / (64 / 8));
                        offset_y = 16;
                        
                        if (map->dis_coin_animation_tick[i][j] == 63) {
                            map->coinStatus[i][j] = DISAPPEAR;
                        }
                    }

                    if (map->coinStatus[i][j] == DISAPPEAR) {
                        map->map[i][j] = FLOOR;
                    }

                    al_draw_scaled_bitmap(map->coin_assets,
                        offset_x, offset_y, 16, 16,
                        dx, dy, TILE_SIZE, TILE_SIZE,
                        0);
                    break;
                }   
            case TROPHY: {
                int offset_x = 0;
                int offset_y = 0;

                offset_x = 32 * (map->trophy_animation_tick / (64 / 8));
                offset_y = 0;

                al_draw_scaled_bitmap(map->trophy_assets,
                    offset_x, offset_y, 32, 32,
                    dx, dy, TILE_SIZE, TILE_SIZE,
                    0);
                break;
            }
            
            case APPLE: {
                if (appleStat) {
                    int offset_x = 0;
                    int offset_y = 0;

                    if (map->appleStatus[i][j] == APPLE_APPEAR) {
                        offset_x = 32 * (map->apple_animation_tick / (64 / 8));
                        offset_y = 0;
                    }

                    if (map->appleStatus[i][j] == APPLE_DISAPPEARING) {
                        map->dis_apple_animation_tick[i][j] =
                            (map->dis_apple_animation_tick[i][j] + 1) % 64;
                        offset_x = 32 * (map->dis_apple_animation_tick[i][j] / (64 / 8));
                        offset_y = 32;

                        if (map->dis_apple_animation_tick[i][j] == 63) {
                            map->appleStatus[i][j] = APPLE_DISAPPEAR;
                        }
                    }

                    if (map->appleStatus[i][j] == APPLE_DISAPPEAR) {
                        map->map[i][j] = FLOOR;
                        appleStat = 0;
                    }

                    al_draw_scaled_bitmap(map->apple_assets,
                        offset_x, offset_y, 32, 32,
                        dx, dy, TILE_SIZE, TILE_SIZE,
                        0);
                
                }
                
                break;
            }

            case BANANA: {
                if (bananaStat) {
                    int offset_x = 0;
                    int offset_y = 0;

                    if (map->bananaStatus[i][j] == BANANA_APPEAR) {
                        offset_x = 32 * (map->banana_animation_tick / (64 / 8));
                        offset_y = 0;
                    }

                    if (map->bananaStatus[i][j] == BANANA_DISAPPEARING) {
                        map->dis_banana_animation_tick[i][j] =
                            (map->dis_banana_animation_tick[i][j] + 1) % 64;
                        offset_x = 32 * (map->dis_banana_animation_tick[i][j] / (64 / 8));
                        offset_y = 32;

                        if (map->dis_banana_animation_tick[i][j] == 63) {
                            map->bananaStatus[i][j] = BANANA_DISAPPEAR;
                        }
                    }

                    if (map->bananaStatus[i][j] == BANANA_DISAPPEAR) {
                        map->map[i][j] = FLOOR;
                        bananaStat = 0;
                    }

                    al_draw_scaled_bitmap(map->banana_assets,
                        offset_x, offset_y, 32, 32,
                        dx, dy, TILE_SIZE, TILE_SIZE,
                        0);
                }      
                break;
            }

            case DOOR_CLOSE: {
                
                if (map->doorStatus == CLOSE) {
                    al_draw_scaled_bitmap(map->door_assets,
                        0, 0, 16, 16,
                        dx, dy, TILE_SIZE, TILE_SIZE,
                        0
                    );
                }
                
                int offset_x = 0;
                int offset_y = 0;
                if (map->doorStatus == OPEN) {
                    map->door_animation_tick = (map->door_animation_tick + 1) % 64;
                    offset_x = 16 * (map->door_animation_tick / (64 / 7));
                    offset_y = 0;

                    if (map->door_animation_tick == 63) {
                        map->map[i][j] = DOOR_OPEN;
                    }

                    al_draw_scaled_bitmap(map->door_assets,
                        offset_x, offset_y,
                        16, 16,
                        dx, dy,
                        TILE_SIZE, TILE_SIZE, 0
                    );
                }
                break;
            }

            case DOOR_OPEN: {
                al_draw_scaled_bitmap(map->door_assets,
                    0, 16, 16, 16,
                    dx, dy, TILE_SIZE, TILE_SIZE, 0
                );
                break;
            }

            case BUTTON: {
                if (map->buttonStatus == UNPRESSED) {
                    al_draw_scaled_bitmap(
                        map->button_assets,
                        0, 0, 16, 16,
                        dx, dy, TILE_SIZE, TILE_SIZE, 0
                    );
                }

                if (map->buttonStatus == PRESSING) {
                    map->button_animation_tick = (map->button_animation_tick + 1) % 64;

                    int offset_x = 16 * (map->button_animation_tick / (64 / 3));
                    int offset_y = 0;

                    if (map->button_animation_tick == 63) {
                        map->buttonStatus = PRESSED;
                    }

                    al_draw_scaled_bitmap(
                        map->button_assets,
                        offset_x, offset_y,
                        16, 16,
                        dx, dy,
                        TILE_SIZE, TILE_SIZE, 0
                    );
                }

                if (map->buttonStatus == PRESSED) {
                    al_draw_scaled_bitmap(
                        map->button_assets,
                        32, 0,
                        16, 16,
                        dx, dy,
                        TILE_SIZE, TILE_SIZE,
                        0
                    );
                }

                break;
            }
               
            case KEY: {
                if (map->chestStatus == CHEST_CLOSED && key_obtained == 0) {
                    al_draw_scaled_bitmap(map->key_assets,
                        0, 0, 19, 10,
                        dx, dy, TILE_SIZE, TILE_SIZE/2, 0
                    );
                    
                }
                break;
               
            }
            
            case CHEST: {
                if (map->chestStatus == CHEST_CLOSED) {
                    al_draw_scaled_bitmap(map->chest_assets,
                        0, 0, 21, 18,
                        dx, dy, 63, 62, 0
                    );
                }

                if (map->chestStatus == CHEST_OPENING) {
                    map->chest_animation_tick = (map->chest_animation_tick + 1) % 64;

                    int offset_x = 21 * (map->chest_animation_tick / (64/2));
                    int offset_y = 0;

                    if (map->chest_animation_tick == 63) {
                        map->chestStatus = CHEST_OPENED;
                        key_obtained = 0;

                    }

                    al_draw_scaled_bitmap(map->chest_assets,
                        offset_x, offset_y, 21, 18,
                        dx, dy, 63, 62, 0
                    );
                }

                if (map->chestStatus == CHEST_OPENED) {
                    map->map[i][j] = COIN;
                }

                break;
            }
            
            case CHEST_COIN: {
                if (map->chestStatus == OPENED && key_obtained == 1) {
                    map->map[i][j] = COIN;
                    map->coinStatus[i][j] = APPEAR;
                }
                else {
                    map->map[i][j] = FLOOR;
                }
                break;
            }
                default:
                    break;
            }
        #ifdef DRAW_HITBOX
            al_draw_rectangle(
                dx, dy, dx + TILE_SIZE, dy + TILE_SIZE,
                al_map_rgb(0, 255, 0), 1
            );
        #endif
        }
    }
}

void update_map(Map * map, Point player_coord, int* total_coins){
    /*
        Hint: To check if it's collide with object in map, you can use tile_collision function
        e.g. to update the coins if you touch it
    */
    map->coin_animation_tick = (map->coin_animation_tick + 1) % 64;
    map->trophy_animation_tick = (map->trophy_animation_tick + 1) % 64;
    map->apple_animation_tick = (map->apple_animation_tick + 1) % 64;
    map->banana_animation_tick = (map->banana_animation_tick + 1) % 64;

    int playerCoord_x = (player_coord.x + TILE_SIZE / 2) / TILE_SIZE;
    int playerCoord_y = (player_coord.y + TILE_SIZE / 2) / TILE_SIZE;
    
    
    if (map->map[playerCoord_y][playerCoord_x] == COIN && 
        map->coinStatus[playerCoord_y][playerCoord_x] == APPEAR) {
            map->coinStatus[playerCoord_y][playerCoord_x] = DISAPPEARING;
            al_play_sample(map->coin_audio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            *total_coins += 1;
    }

    if (map->map[playerCoord_y][playerCoord_x] == KEY && map->chestStatus == CHEST_CLOSED) {
        al_play_sample(map->key_audio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        key_obtained = 1;
    }

    if (key_obtained && map->map[playerCoord_y][playerCoord_x] == CHEST && map->chestStatus == CHEST_CLOSED) {
        al_play_sample(map->chest_audio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        map->chestStatus = CHEST_OPENING;
        *total_coins += 3;
        
    }


    if (map->map[playerCoord_y][playerCoord_x] == BUTTON &&
        map->buttonStatus == UNPRESSED) {
        map->buttonStatus = PRESSING;
        map->doorStatus = OPEN;
        al_play_sample(map->button_audio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_play_sample(map->door_audio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }

    if (map->map[playerCoord_y][playerCoord_x] == TROPHY) {
        win = 1;
    }

    if (map->map[playerCoord_y][playerCoord_x] == APPLE && 
        map->appleStatus[playerCoord_y][playerCoord_x] == APPLE_APPEAR) {
        if (appleStat) {
            map->appleStatus[playerCoord_y][playerCoord_x] = DISAPPEARING;
            al_play_sample(map->apple_audio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            apple = 1;
            appleStat = 0;
            applePrice = 1;
        }
            
    }

    if (map->map[playerCoord_y][playerCoord_x] == BANANA &&
        map->bananaStatus[playerCoord_y][playerCoord_x] == BANANA_APPEAR) {
        if (bananaStat) {
            map->bananaStatus[playerCoord_y][playerCoord_x] = DISAPPEARING;
            al_play_sample(map->banana_audio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            banana = 1;
            bananaStat = 0;
            bananaPrice = 1;
        }      
    }

}

void destroy_map(Map * map){
    for(int i = 0; i < map->row; i++){
        free(map->map[i]);
        free(map->offset_assets[i]);
    }
    free(map->map);
    free(map->offset_assets);

    al_destroy_bitmap(map->assets);
    al_destroy_bitmap(map->coin_assets);
    al_destroy_bitmap(map->trophy_assets);
    al_destroy_bitmap(map->apple_assets);
    al_destroy_bitmap(map->banana_assets);
    al_destroy_sample(map->coin_audio);
    al_destroy_sample(map->apple_audio);
    al_destroy_sample(map->banana_audio);

}

bool isWalkable(BLOCK_TYPE block){
    if(block == FLOOR ||  block == COIN || block == TROPHY || block == APPLE || block == BANANA || 
        block == BUTTON || block == DOOR_OPEN || block == CHEST || block == KEY || block == CHEST_COIN) return true;
    return false;
}

/*
    DON'T CHANGE CODE BELOW UNLESS YOU ARE UNDERSTAND WELL
    OR WANT TO CHANGE TO DIFFERENT ASSETS
 */
static bool isWall(Map * map, int i, int j);
//static bool isFloorHole(Map * map, int i, int j);
static bool isFloor(Map * map, int i, int j);
static bool isNothing(Map * map, int i, int j);

static Point get_floor_offset_assets(Map * map, int i, int j){
    bool up = isWall(map, i-1, j);
    bool left = isWall(map, i, j-1);
    bool right = isWall(map, i, j+1);
    bool top_left = isWall(map, i-1, j-1);
    bool top_right = isWall(map, i-1, j+1);
    
    if(up && left && right){
        return (Point){ offset * 12, offset * 3 };
    }
    if(up && left){
        return (Point){ offset * 11, 0 };
    }
    if(up && right){
        return (Point){ offset * 13, 0 };
    }
    if(left){
        if(top_left)
            return (Point){ offset * 11, offset * 1 };
        else
            return (Point){ offset * 15, offset * 1 };
        
    }
    if(right){
        if(top_right)
            return (Point){ offset * 13, offset * 1 };
        else
            return (Point){ offset * 14, offset * 1 };
    }
    if(up){
        return (Point){ offset * 12, 0 };
    }
    if(top_left){
        return (Point){ offset * 11, offset * 3 };
    }
    if(top_right){
        return (Point){ offset * 13, offset * 3 };
    }
    
    return (Point){12 * offset, 4 * offset};
}

// Calculate the offset from the source assets
static Point get_wall_offset_assets(Map * map, int i, int j){
    bool up = isWall(map, i-1, j);
    bool down = isWall(map, i+1, j);
    bool left = isWall(map, i, j-1);
    bool right = isWall(map, i, j+1);

    if(up && down && left && right){
        return (Point){ 3 * offset, 5 * offset };
    }
    if(up && down && right){
        bool left_floor = isFloor(map, i, j - 1);
        bool right_down = isWall(map, i + 1, j + 1);
        bool right_up = isWall(map, i - 1, j + 1);
        if (right_down && right_up)
            return (Point) { 2 * offset, 5 * offset };
        if (left_floor) {
            return (Point) { 1 * offset, 3 * offset };
        }
        else {
            return (Point) { 4 * offset, 5 * offset };
        }
    }
    if (up && down && left) {
        bool right_floor = isFloor(map, i, j + 1);
        bool left_down = isWall(map, i + 1, j - 1);
        bool left_up = isWall(map, i - 1, j - 1);
        if (left_down && left_up)
            return (Point) { 4 * offset, 5 * offset };
        if (right_floor) {
            return (Point) { 1 * offset, 3 * offset };
        }
        else {
            return (Point) { 2 * offset, 5 * offset };
        }
    }
    if(down && right && left){
        bool down_right_wall = isWall(map, i + 1, j + 1);
        bool down_left_wall = isWall(map, i + 1, j - 1);
        bool down_right = isFloor(map, i+1, j+1);
        bool down_left = isFloor(map, i+1, j-1);
        if(down_right_wall && down_left_wall)
            return (Point) { 3 * offset, 4 * offset };
        if((down_right ^ down_left) == 0)
            return (Point){ 8 * offset, 5 * offset };
        if(down_right)
            return (Point){ 4 * offset, 4 * offset };
        if(down_left)
            return (Point){ 2 * offset, 4 * offset };
    }
    if(left && right){
        if(isFloor(map, i+1, j))
            return (Point){ 7 * offset, 5 * offset};
        else
            return (Point){ 7 * offset, 7 * offset};
    }
    if(down && up){
        bool left_ = isFloor(map, i, j-1);
        bool right_ = isFloor(map, i, j+1);
        if((left_ ^ right_) == 0)
            return (Point){ 1 * offset, 3 * offset};
        if(left_)
            return (Point){ 2 * offset, 5 * offset};
        return (Point){ 4 * offset, 5 * offset};
    }
    if(down && right){
        if(isFloor(map, i+1, j+1))
            return (Point){ 4 * offset, 5 * offset };
        else
            return (Point){ 2 * offset, 4 * offset };
    }
    if(down && left){
        if (isFloor(map, i, j + 1) && isFloor(map, i + 1, j - 1))
            return (Point) { 1 * offset, 2 * offset };
        else if(isFloor(map, i+1, j-1))
            return (Point){ 2 * offset, 5 * offset };
        else
            return (Point){ 4 * offset, 4 * offset};
    }
    if(up && right){
        if(isFloor(map, i+1, j))
            return (Point){ 2 * offset, 6 * offset};
        else
            return (Point){ 5 * offset, 6 * offset};
    }
    if(up && left){
        if(isFloor(map, i+1, j))
            return (Point){ 4 * offset, 6 * offset};
        else
            return (Point){ 6 * offset, 6 * offset};
    }
    if(left || right){
        if(isFloor(map, i+1, j))
            return (Point){ 7 * offset, 5 * offset};
        else
            return (Point){ 7 * offset, 7 * offset};
    }
    if(down){
        return (Point){ 1 * offset, 2 * offset};
    }
    if(up){
        return (Point){ 1 * offset, 4 * offset};
    }
    
    return (Point){ 0, 0 };
}

static Point get_hole_offset_assets(Map * map, int i, int j){
    bool up = isNothing(map, i-1, j);
    bool U_Floor = isFloor(map, i-1, j);
    
    if(up){
        return (Point){ 0, 0 };
    }
    if(U_Floor){
        bool UL = isFloor(map, i-1, j-1);
        bool UR = isFloor(map, i-1, j+1);
        if(UL && UR) return (Point){ offset * 13, offset * 11 };
        if(UL) return (Point){ offset * 14, offset * 11 };
        if(UR) return (Point){ offset * 12, offset * 11 };
        return (Point){ offset * 10, offset * 12 };
    }
    return (Point){ 0, 0 };
}

static Point get_nothing_offset_assets(Map * map, int i, int j){
    bool U_Floor = isFloor(map, i-1, j);
    if(U_Floor){
        bool UL = isFloor(map, i-1, j-1);
        bool UR = isFloor(map, i-1, j+1);
        if(UL && UR) return (Point){ offset * 13, offset * 11 };
        if(UL) return (Point){ offset * 14, offset * 11 };
        if(UR) return (Point){ offset * 12, offset * 11 };
        return (Point){ offset * 10, offset * 12 };
    }
    return (Point){ 0, 0 };
}

static void get_map_offset(Map * map){
    // Calculate once to use it later on when draw() function called
    for(int i = 0; i < map->row; i++){
        for(int j = 0; j < map->col; j++){
            switch(map->map[i][j]){
                case WALL:
                    map->offset_assets[i][j] = get_wall_offset_assets(map, i, j);
                    break;
                case FLOOR:
                case COIN:
                    map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                    break;

                case HOLE:
                    map->offset_assets[i][j] = get_hole_offset_assets(map, i, j);
                    break;

                case TROPHY:
                    map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                    break;

                case APPLE:
                    map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                    break;

                case BANANA:
                    map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                    break;

                case DOOR_CLOSE:
                    map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                    break;

                case DOOR_OPEN:
                    map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                    break;

                case CHEST:
                    map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                    break;

                case KEY:
                    map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                    break;

                case CHEST_COIN:
                    map->offset_assets[i][j] = get_floor_offset_assets(map, i, j);
                    break;

                case NOTHING:
                default:
                    map->offset_assets[i][j] = (Point){ 0, 0 };
                    break;
            }
        }
    }
}

static bool isWall(Map * map, int i, int j){
    if(i < 0 || j < 0 || i >= map->row || j >= map->col) return false;
    if(map->map[i][j] == WALL) return true;
    return false;
}

static bool isFloor(Map * map, int i, int j){
    if(i < 0 || j < 0 || i >= map->row || j >= map->col) return false;
    if(map->map[i][j] == FLOOR) return true;
    return false;
}

static bool isNothing(Map * map, int i, int j){
    if(i < 0 || j < 0 || i >= map->row || j >= map->col) return true;
    if(map->map[i][j] == NOTHING || map->map[i][j] == HOLE) return true;
    return false;
}

/*
    Collision
 */
static bool tile_collision(Point player, Point tile_coord){
    
    if (player.x < tile_coord.x + TILE_SIZE &&
        player.x + TILE_SIZE > tile_coord.x &&
        player.y < tile_coord.y + TILE_SIZE &&
        player.y + TILE_SIZE > tile_coord.y) {
            return true; // Rectangles collide
    } else {
        return false; // Rectangles do not collide
    }
}
