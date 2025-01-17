#include <string.h>
#include "game_scene.h"
#include "menu_scene.h"
#include "loading_scene.h"
#include "submit_scene.h"
#include "game.h"
#include "player.h"
#include "map.h"
#include "enemy.h"
#include "weapon.h"
#include "UI.h"

#include <math.h>

Player player; // Player
Map map; // Map
enemyNode * enemyList; // Enemy List
BulletNode * bulletList; // Bullet List

int win = 0;
int lose = 0;

int weaponStat, sniperStat;

// Weapon
Weapon weapon; 
int coins_obtained = 0;
int totalCoins = 0;

int paused = 0;

ALLEGRO_BITMAP* healthBitmap;
ALLEGRO_BITMAP* coinBitmap;
ALLEGRO_BITMAP* winBitmap;
ALLEGRO_BITMAP* loseBitmap;
ALLEGRO_BITMAP* pauseBitmap;
ALLEGRO_BITMAP* pauseMenu;
ALLEGRO_BITMAP* blur;
ALLEGRO_BITMAP* pandaLose;
ALLEGRO_BITMAP* pandaWin;

static Button playAgainButton;
static Button menuButton;
static Button nextLevelButton;
static Button pauseButton;
static Button pauseButtonHov;
static Button continueButton;
static Button menuButton;
static Button submitButton;

int currentLevel = 1;
int currentHealth = 50;
int applePrice, bananaPrice;

static void init(void){
    initEnemy();

    paused = 0;
    pauseButton = button_create(710, 20, TILE_SIZE, TILE_SIZE, "Assets/UI_squareButton_2.png", "Assets/UI_squareButton_hovered_2.png");
    pauseButtonHov = button_create(710, 20, TILE_SIZE, TILE_SIZE, "Assets/UI_squareButton_hovered_2.png", "Assets/UI_squareButton_hovered_2.png");
    continueButton = button_create(SCREEN_W / 2 - 150, 315, 300, 75, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    menuButton = button_create(SCREEN_W / 2 - 150, 400, 300, 75, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    
    coins_obtained = 0;
    healthBitmap = al_load_bitmap("Assets/heart.png");
    coinBitmap = al_load_bitmap("Assets/coin_icon.png");
    pauseBitmap = al_load_bitmap("Assets/pause.png");
    pauseMenu = al_load_bitmap("Assets/pause_menu.png");
    blur = al_load_bitmap("Assets/blur.jpg");

    char currentMap[50];
    snprintf(currentMap, sizeof(currentMap), "Assets/map%d.txt", currentLevel);
    map = create_map(currentMap, 0);

    player = create_player("Assets/panda2.png", map.Spawn.x, map.Spawn.y, currentHealth);

    enemyList = createEnemyList();
    bulletList = createBulletList();

    if (weaponStat) {
        weapon = create_weapon("Assets/guns.png", "Assets/yellow_bullet.png", 16, 8, 10);
    }
    else if (sniperStat) {
        weapon = create_weapon("Assets/sniper.png", "Assets/orange_bullet.png", 100, 5, 50);
    }

    
    
    for(int i=0; i<map.EnemySpawnSize; i++){
        Enemy enemy = createEnemy(map.EnemySpawn[i].x, map.EnemySpawn[i].y, map.EnemyCode[i]);
        insertEnemyList(enemyList, enemy);
    }

    game_log("coord x:%d \n coords y:%d \n", map.Spawn.x, map.Spawn.y);
    change_bgm("Assets/audio/game_bgm.mp3");
}

static void update(void){
    /*
        [TODO Homework] DONE
        
        Change the scene if winning/losing to win/lose scene
    */
    update_button(&pauseButton);
    update_button(&pauseButtonHov);

    if (paused) {
        update_button(&continueButton);
        update_button(&menuButton);
        if ((mouseState.buttons && pauseButtonHov.hovered == true) || keyState[ALLEGRO_KEY_ESCAPE] || 
            (mouseState.buttons && continueButton.hovered == true)) {
            paused = 0;
            al_rest(0.2);
        }
        else if (mouseState.buttons && menuButton.hovered == true) {
            change_scene(create_menu_scene());
            al_rest(0.2);
        }
        return;
    }

    if ((mouseState.buttons && pauseButton.hovered == true) || keyState[ALLEGRO_KEY_ESCAPE]) {
        paused = 1;
        al_rest(0.2);
        return;
    }
    
    
    if (win) {
        al_play_sample(map.trophy_audio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        change_scene(create_win_scene());
        currentLevel += 1;
        win = 0;
        totalPoints += (coins_obtained == 0) ? currentHealth : coins_obtained * currentHealth;
        totalCoins += coins_obtained;
        currentHealth = player.health;
        bananaStat = 0;
        appleStat = 0;
        
        return;

    }
    else if (lose) {
        al_play_sample(map.lose_audio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        change_scene(create_lose_scene());
        lose = 0;
        coins_obtained = 0;
        totalCoins += coins_obtained;
        return;
    }


    update_player(&player, &map);

    Point Camera;
    /*
        [TODO HACKATHON 1-3] DONE
        
        Calcualte the formula for the Camera
        Camera.x = ...
        Camera.y = ...

        Hint: Adjust it based on player position variable, then subtract it with half of the gameplay screen
    */
    Camera.x = player.coord.x - (SCREEN_W/2) + (TILE_SIZE/2);
    Camera.y = player.coord.y - (SCREEN_H/2) + (TILE_SIZE/2);



    updateEnemyList(enemyList, &map, &player);
    update_weapon(&weapon, bulletList, player.coord, Camera);
    updateBulletList(bulletList, enemyList, &map);
    update_map(&map, player.coord, &coins_obtained);
    
}

static void draw(void){
    Point Camera;
    /*
        [TODO HACKATHON 1-4] DONE
        
        Calcualte the formula for the Camera
        Camera.x = ...
        Camera.y = ...

        Hint: Just copy from the [TODO HACKATHON 1-3]
    */
    Camera.x = player.coord.x - (SCREEN_W/2) + (TILE_SIZE/2);
    Camera.y = player.coord.y - (SCREEN_H/2) + (TILE_SIZE/2);
    
    // Draw
    draw_map(&map, Camera);
    drawEnemyList(enemyList, Camera);
    drawBulletList(bulletList, Camera);
    draw_player(&player, Camera);

    draw_weapon(&weapon, player.coord, Camera);

   

    if (paused) {
        ALLEGRO_COLOR continueColor = continueButton.hovered ? al_map_rgb(255, 182, 0) : al_map_rgb(255, 255, 255);
        ALLEGRO_COLOR menuColor = menuButton.hovered ? al_map_rgb(255, 182, 0) : al_map_rgb(255, 255, 255);

        al_draw_tinted_scaled_bitmap(blur, al_map_rgb(225, 225, 225),
            0, 0, 800, 800,
            0, 0, SCREEN_W+100, SCREEN_H+100,
            0
        );
        draw_button(pauseButtonHov);
        al_draw_tinted_scaled_bitmap(pauseBitmap, al_map_rgb(66, 76, 110),
            0, 0, 800, 800,
            730, 39, 25, 25,
            0
        );
        al_draw_tinted_scaled_bitmap(pauseBitmap, al_map_rgb(225, 225, 225),
            0, 0, 800, 800,
            730, 42, 25, 25,
            0
        );
        
        al_draw_tinted_scaled_bitmap(pauseMenu, al_map_rgb(225, 225, 225),
            0, 0, 400, 400,
            SCREEN_W/2 - 200, SCREEN_H/2 - 150, 400, 400,
            0
        );
        al_draw_text(
            P2_FONT,
            al_map_rgb(66, 76, 110),
            SCREEN_W / 2,
            272,
            ALLEGRO_ALIGN_CENTER,
            "PAUSED"
        );
        al_draw_text(
            P2_FONT,
            al_map_rgb(225, 225, 225),
            SCREEN_W / 2,
            275,
            ALLEGRO_ALIGN_CENTER,
            "PAUSED"
        );

        al_draw_text(
            P2_FONT,
            al_map_rgb(66, 76, 110),
            SCREEN_W / 2,
            350,
            ALLEGRO_ALIGN_CENTER,
            "CONTINUE"
        );
        al_draw_text(
            P2_FONT,
            continueColor,
            SCREEN_W / 2,
            350 + 3,
            ALLEGRO_ALIGN_CENTER,
            "CONTINUE"
        );

        al_draw_text(
            P2_FONT,
            al_map_rgb(66, 76, 110),
            SCREEN_W / 2,
            420,
            ALLEGRO_ALIGN_CENTER,
            "MENU"
        );
        al_draw_text(
            P2_FONT,
            menuColor,
            SCREEN_W / 2,
            420 + 3,
            ALLEGRO_ALIGN_CENTER,
            "MENU"
        );
    }
    else {
        draw_button(pauseButton);
        al_draw_tinted_scaled_bitmap(pauseBitmap, al_map_rgb(66, 76, 110),
            0, 0, 800, 800,
            730, 33 + pauseButton.hovered * 6, 25, 25,
            0
        );
        al_draw_tinted_scaled_bitmap(pauseBitmap, al_map_rgb(225, 225, 225),
            0, 0, 800, 800,
            730, 33 + 3 + pauseButton.hovered * 6, 25, 25,
            0
        );
    }

    /*
        [TODO Homework]
        
        Draw the UI of Health and Total Coins
    */
    //HEALTH
    int healthMax = player.health / 10;
    int i = 0;
    while(i < healthMax) {
        al_draw_tinted_scaled_bitmap(healthBitmap, al_map_rgb(255, 255, 255),
            0, 0, 32, 32, // source image x, y, width, height
            10 + (i * 50), 10, TILE_SIZE, TILE_SIZE, // destiny x, y, width, height
            0 // Flip or not
        );
        i++;
    }
    //TOTAL COINS
    al_draw_tinted_scaled_bitmap(coinBitmap, al_map_rgb(255, 255, 255),
        0, 0, 32, 32, // source image x, y, width, height
        10, 60, TILE_SIZE*2, TILE_SIZE*2, // destiny x, y, width, height
        0 // Flip or not
    );
    
    char coin[100];
    snprintf(coin, sizeof(coin), "%02d", totalCoins + coins_obtained);

    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        77, 77, 0,
        coin
    );
    
}

static void destroy(void){
    delete_player(&player);
    delete_weapon(&weapon);
    destroy_map(&map);
    destroyBulletList(bulletList);
    destroyEnemyList(enemyList);
    terminateEnemy();
}


Scene create_game_scene(void){
    Scene scene;
    memset(&scene, 0, sizeof(Scene));
    
    scene.name = "game";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;
    
    return scene;
}

static void init_win(void) {
    winBitmap = al_load_bitmap("Assets/win.png");
    menuButton = button_create(SCREEN_W / 2 - 100, 250, 200, 75, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    nextLevelButton = button_create(SCREEN_W / 2 - 100, 415, 200, 75, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    submitButton = button_create(SCREEN_W / 2 - 100, 335, 200, 75, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    pandaWin = al_load_bitmap("Assets/panda_win.png");
}

static void init_lose(void) {
    loseBitmap = al_load_bitmap("Assets/lose.png");
    playAgainButton = button_create(SCREEN_W / 2 - 225, SCREEN_H/2 - 60, 200, 75, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    menuButton = button_create(SCREEN_W / 2 + 45, SCREEN_H / 2 - 60, 200, 75, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    pandaLose = al_load_bitmap("Assets/panda_lose.png");
}

static void draw_win(void) {
    al_draw_scaled_bitmap(winBitmap,
        0, 0,
        800, 800,
        0, 0,
        800, 800, 0
    );
    
    al_draw_tinted_scaled_bitmap(pandaWin, al_map_rgb(255, 255, 255),
        0, 0, 64, 64, // source image x, y, width, height
        SCREEN_W / 2 - (TILE_SIZE * 6 / 2), 425, TILE_SIZE * 6, TILE_SIZE * 6, // destiny x, y, width, height
        0 // Flip or not
    );

    if (currentLevel <= 3) {
        draw_button(menuButton);

        al_draw_text(
            P2_FONT,
            al_map_rgb(66, 76, 110), //Shadow
            SCREEN_W / 2,
            270 + menuButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "MENU"
        );
        al_draw_text(
            P2_FONT,
            al_map_rgb(255, 255, 255),
            SCREEN_W / 2,
            270 + 3 + menuButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "MENU"
        );

        draw_button(nextLevelButton);

        al_draw_text(
            P2_FONT,
            al_map_rgb(66, 76, 110), //Shadow
            SCREEN_W / 2,
            435 + nextLevelButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "NEXT"
        );
        al_draw_text(
            P2_FONT,
            al_map_rgb(255, 255, 255),
            SCREEN_W / 2,
            435 + 3 + nextLevelButton.hovered * 11,
            ALLEGRO_ALIGN_CENTER,
            "NEXT"
        );
    }

    draw_button(submitButton);
    
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110), //Shadow
        SCREEN_W / 2,
        355 + submitButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SUBMIT"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        SCREEN_W / 2,
        355 + 3 + submitButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "SUBMIT"
    );

}

static void draw_lose(void) {

    al_draw_tinted_scaled_bitmap(loseBitmap, al_map_rgb(255, 255, 255),
        0, 0, 800, 800, // source image x, y, width, height
        0, 0, 800, 800, // destiny x, y, width, height
        0 // Flip or not
    );

    al_draw_tinted_scaled_bitmap(pandaLose, al_map_rgb(255, 255, 255),
        0, 0, 64, 64, // source image x, y, width, height
        SCREEN_W / 2 - (TILE_SIZE * 6 / 2), 425, TILE_SIZE * 6, TILE_SIZE * 6, // destiny x, y, width, height
        0 // Flip or not
    );

    draw_button(playAgainButton);
    draw_button(menuButton);
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2 - 160,
        SCREEN_H / 2 - 43 + playAgainButton.hovered * 11,
        0,
        "YES"
    );

    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        SCREEN_W/2 - 160,
        SCREEN_H/2 - 43 + 3 + playAgainButton.hovered*11,
        0,
        "YES"
    );

    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110), //Shadow
        SCREEN_W / 2 + 150,
        SCREEN_H / 2 - 43 + menuButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "NO"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        SCREEN_W / 2 + 150,
        SCREEN_H / 2 - 43 + 3 + menuButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "NO"
    );
}

static void update_win(void) {
    draw_win();
    update_button(&submitButton);
    update_button(&menuButton);
    update_button(&nextLevelButton);

    if (submitButton.hovered && mouseState.buttons == 1) {
        change_scene(create_submit_scene());
        al_rest(0.2);
        return;
    }

    if (menuButton.hovered && mouseState.buttons == 1) {
        change_scene(create_menu_scene());
        al_rest(0.2);
        return;
    }

    if (nextLevelButton.hovered && mouseState.buttons == 1) {
        change_scene(create_loading_scene());
        al_rest(0.2);
        return;
    }

}

static void update_lose(void) {
    draw_lose();
    update_button(&menuButton);
    update_button(&playAgainButton);
    update_button(&submitButton);

    if (menuButton.hovered && mouseState.buttons == 1) {
        change_scene(create_menu_scene());
        al_rest(0.2);
        return;
    }

    if (playAgainButton.hovered && mouseState.buttons == 1) {
        change_scene(create_game_scene());
        al_rest(0.2);
        return;
    }

    if (submitButton.hovered && mouseState.buttons == 1) {
        change_scene(create_submit_scene());
        al_rest(0.2);
        return;
    }
}

static void destroy_win(void) {
    al_destroy_bitmap(winBitmap);
    al_destroy_bitmap(coinBitmap);
    al_destroy_bitmap(healthBitmap);
    destroy_button(&playAgainButton);
    destroy_button(&menuButton);
}

static void destroy_lose(void) {
    al_destroy_bitmap(loseBitmap);
    destroy_button(&menuButton);
}

Scene create_win_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "win";
    scene.init = &init_win;
    scene.draw = &draw_win;
    scene.update = &update_win;
    scene.destroy = &destroy_win;

    return scene;
}

Scene create_lose_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "lose";
    scene.init = &init_lose;
    scene.draw = &draw_lose;
    scene.update = &update_lose;
    scene.destroy = &destroy_lose;

    return scene;
}


