#include "player.h"
#include "game.h"
#include "utility.h"
#include "map.h"
#include "game_scene.h"
#include <math.h>

static bool isCollision(Player* player, Map* map);

int flag = 0; // Change the flag to flip character
int apple = 0;
int banana = 0;

Player create_player(char * path, int row, int col, int health){
    Player player;
    memset(&player, 0, sizeof(player));
    

    player.coord = (Point){
        col * TILE_SIZE,
        row * TILE_SIZE
    };
    
    player.speed = 4;
    player.health = health;
    player.speed_timer = 0;
    
    player.image = al_load_bitmap(path);
    if(!player.image){
        game_abort("Error Load Bitmap with path : %s", path);
    }
    
    return player;
}

void update_player(Player * player, Map* map){

    Point original = player->coord;

    if (player->status == PLAYER_DYING) {
        player->animation_tick = (player->animation_tick + 1) % 64; //biar ttp keanimate
        if (player->animation_tick == 63) {
            player->animation_tick = 62;
        }
        lose = 1;
        return;
    }

    if (apple) {
        player->health = player->health + 10;
        apple = 0;
    }

    if (banana) {
        player->speed = player->speed * 2; 
        player->speed_timer = 300; // 5 seconds at 60 FPS
        banana = 0;
    }
    
    //Reset to normal
    if (player->speed_timer > 0) {
        player->speed_timer--;
        if (player->speed_timer == 0) {
            player->speed = 4;
        }
    }
    
    // Knockback effect
    if(player->knockback_CD > 0){

        player->knockback_CD--;
        int next_x = player->coord.x + player->speed * cos(player->knockback_angle);
        int next_y = player->coord.y + player->speed * sin(player->knockback_angle);
        Point next;
        next = (Point){next_x, player->coord.y};

        
        
        if(!isCollision(player, map)){
            player->coord = next;
        }
        
        next = (Point){player->coord.x, next_y};
        if(!isCollision(player, map)){
            player->coord = next;
        }
    }
    
    /*
        [TODO HACKATHON 1-1] DONE
        
        Player Movement
        Adjust the movement by player->speed

        if (keyState[ALLEGRO_KEY_W]) {
            player->coord.y = ...
            player->direction = ...
        }
        if (keyState[ALLEGRO_KEY_S]) {
            player->coord.y = ...
            player->direction = ...
        }
    */
    player->status = PLAYER_IDLE;
    if (keyState[ALLEGRO_KEY_W] || keyState[ALLEGRO_KEY_UP]) {
        player->coord.y = player->coord.y - player->speed;
        player->direction = UP;
        player->status = PLAYER_WALKING;
    }
    if (keyState[ALLEGRO_KEY_S] || keyState[ALLEGRO_KEY_DOWN]) {
        player->coord.y = player->coord.y + player->speed;
        player->direction = DOWN;
        player->status = PLAYER_WALKING;
    }

    // if Collide, snap to the grid to make it pixel perfect
    if(isCollision(player, map)){ //if collide with the wall
        player->coord.y = round((float)original.y / (float)TILE_SIZE) * TILE_SIZE;
    }
    
    /*
        [TODO HACKATHON 1-2] DONE
        
        Player Movement
        Add/Subtract the movement by player->speed

        if (keyState[ALLEGRO_KEY_A]) {
            player->coord.y = ...
            player->direction = ...
        }
        if (keyState[ALLEGRO_KEY_D]) {
            player->coord.y = ...
            player->direction = ...

    }
    */
    if (keyState[ALLEGRO_KEY_A] || keyState[ALLEGRO_KEY_LEFT]) {
        player->coord.x = player->coord.x - player->speed;
        player->direction = LEFT;
        flag = 0;
        player->status = PLAYER_WALKING;
    }
    if (keyState[ALLEGRO_KEY_D] || keyState[ALLEGRO_KEY_RIGHT]) {
        player->coord.x = player->coord.x + player->speed;
        player->direction = RIGHT;
        flag = 1;
        player->status = PLAYER_WALKING;
    }
    if(isCollision(player, map)){
        player->coord.x = round((float)original.x / (float)TILE_SIZE) * TILE_SIZE;
    }
    
    /*
        [TODO Homework] //DONE
        
        Calculate the animation tick to draw animation later 
    */
    player->animation_tick = (player->animation_tick + 1) % 64;
    game_log("x: %d, y: %d", player->coord.x, player->coord.y);
}

void draw_player(Player * player, Point cam){
    int dy = player->coord.y - cam.y; // destiny y axis
    int dx = player->coord.x - cam.x; // destiny x axis
    
    /*
        [TODO Homework] //DONE
        
        Draw Animation of Dying, Walking, and Idle
    */
    int offset_x;
    int offset_y;
    if (player->status == PLAYER_IDLE) {
        offset_x = 32 * (player->animation_tick / (64 / 2));
        offset_y = 0;
    }

    if (player->status == PLAYER_WALKING) {
        offset_x = 32 * (player->animation_tick / (64 / 4));
        offset_y = 32;
    }

    if (player->status == PLAYER_DYING) {
        offset_x = 32 * (player->animation_tick / (64 / 4));
        offset_y = 64;
    }


    al_draw_tinted_scaled_bitmap(player->image, al_map_rgb(255, 255, 255),
        offset_x, offset_y, 32, 32, // source image x, y, width, height
        dx, dy, TILE_SIZE, TILE_SIZE, // destiny x, y, width, height
        flag // Flip or not
    );

    
#ifdef DRAW_HITBOX
    al_draw_rectangle(
        dx, dy, dx + TILE_SIZE, dy + TILE_SIZE,
        al_map_rgb(255, 0, 0), 1
    );
#endif
    
}

void delete_player(Player * player){
    al_destroy_bitmap(player->image);
}

static bool isCollision(Player* player, Map* map){

    if (player->coord.x < 0 ||
        player->coord.y < 0 ||
        (player->coord.x + TILE_SIZE - 1) / TILE_SIZE >= map->col ||
        (player->coord.y + TILE_SIZE - 1) / TILE_SIZE >= map->row)
        return true;

    /*

        [TODO HACKATHON 2-1] DONE
    
        Check every corner of enemy if it's collide or not

        We check every corner if it's collide with the wall/hole or not
        You can activate DRAW_HITBOX in utility.h to see how the collision work

        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;
        if(!isWalkable(map->map[...][...])) return true;

    */
    int playerCoord_x1 = player->coord.x/TILE_SIZE;
    int playerCoord_y1 = player->coord.y/TILE_SIZE;
    int playerCoord_x2 = (player->coord.x + TILE_SIZE - 1) / TILE_SIZE;
    int playerCoord_y2 = (player->coord.y + TILE_SIZE - 1) / TILE_SIZE;

    if (!isWalkable(map->map[playerCoord_y1][playerCoord_x1])) return true;
    if (!isWalkable(map->map[playerCoord_y2][playerCoord_x2])) return true;
    if (!isWalkable(map->map[playerCoord_y1][playerCoord_x2])) return true;
    if (!isWalkable(map->map[playerCoord_y2][playerCoord_x1])) return true;
    
    return false;
}

void hitPlayer(Player * player, Point enemy_coord, int damage){
    if(player->knockback_CD == 0){
        float dY = player->coord.y - enemy_coord.y;
        float dX = player->coord.x - enemy_coord.x;
        float angle = atan2(dY, dX);

        /*
            [TODO Homework] DONE
            
            Game Logic when the player get hit or die

            player->knockback_angle = angle;
            player->knockback_CD = 32;

            player->health = ...
            if(player->health <= 0){

                player->health = 0;
                player->status = ...

                ...
            }
        */
        player->knockback_angle = angle;
        player->knockback_CD = 32;

        player->health = player->health - damage;

        if (player->health <= 0) {
            player->health = 0;
            player->status = PLAYER_DYING;
            player->animation_tick = 0; //to reset
        }
    }
}
