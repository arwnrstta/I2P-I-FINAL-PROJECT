#include <allegro5/allegro.h>
#include <string.h>
#include "menu_scene.h"
#include "game_scene.h"
#include "setting_scene.h"
#include "shop_scene.h"
#include "ldb_scene.h"
#include "loading_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"
#include "time.h"

static Button settingButton;
static Button playButton;
static Button shopButton;
static Button ldbButton;
static Button startButton;
ALLEGRO_BITMAP* titleBitmap;
ALLEGRO_BITMAP* settingBitmap;
ALLEGRO_BITMAP* startMenu;

ALLEGRO_BITMAP* morningBg;
ALLEGRO_BITMAP* eveningBg;
ALLEGRO_BITMAP* nightBg;
ALLEGRO_BITMAP* currentBg;

ALLEGRO_COLOR playColor;
ALLEGRO_COLOR ldbColor;
ALLEGRO_COLOR shopColor;

ALLEGRO_SAMPLE* pressAudio;

ALLEGRO_BITMAP* stars;
int start;

static void init(void) {
    titleBitmap = al_load_bitmap("Assets/title.png");
    settingBitmap = al_load_bitmap("Assets/settings.png");
    startMenu = al_load_bitmap("Assets/start_menu.png");

    morningBg = al_load_bitmap("Assets/morning_menu.png");
    eveningBg = al_load_bitmap("Assets/evening_menu.png");
    nightBg = al_load_bitmap("Assets/night_menu.png");
    currentBg = NULL;

    stars = al_load_bitmap("Assets/star.png");

    start = 0;
    pressAudio = al_load_sample("Assets/audio/press.mp3");

    playButton = button_create(
        SCREEN_W / 2 - 200, 
        430, 
        400, 50, 
        "Assets/UI_Button.png", "Assets/UI_Button_hovered.png"
    );

    shopButton = button_create(
        SCREEN_W / 2 - 200,
        510,
        400, 50,
        "Assets/UI_Button.png", "Assets/UI_Button_hovered.png"
    );

    ldbButton = button_create(
        SCREEN_W / 2 - 200,
        600,
        400, 50,
        "Assets/UI_Button.png", "Assets/UI_Button_hovered.png"
    );

    settingButton = button_create(
        700,
        20,
        75, 75,
        "Assets/UI_SquareButton_2.png", "Assets/UI_SquareButton_hovered_2.png"
    );

    startButton = button_create(
        SCREEN_W / 2 - 125,
        675,
        250, 85,
        "Assets/start_button.png", "Assets/start_button_hovered.png"
    ); 

    change_bgm("Assets/audio/menu_song.mp3");
}

static void update(void) {
    update_button(&settingButton);
    update_button(&startButton);
    
    star_animation_tick = (star_animation_tick + 1) % 64;

    /*if (keyState[ALLEGRO_KEY_ENTER]) {
        change_scene(create_loading_scene());
    }*/

    /*
        [TODO HACKATHON 4-3] DONE
        
        Change scene to setting scene when the button is pressed
    */
    time_t now = time(NULL);
    struct tm* localTime = localtime(&now);

    int hour = localTime->tm_hour;
    int minute = localTime->tm_min;

    if (hour >= 6 && hour < 15) {
        currentBg = morningBg;
    }
    else if ((hour == 15 && minute >= 10) || (hour > 15 && hour < 18)) {
        currentBg = eveningBg;
    }
    else {
        currentBg = nightBg;
    }

    if (settingButton.hovered && mouseState.buttons == 1) {
        al_play_sample(pressAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        change_scene(create_setting_scene());
        al_rest(0.2);
    }

    if (startButton.hovered && mouseState.buttons == 1) {
        if (start == 0) {
            start = 1;
            al_play_sample(pressAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            al_rest(0.2);
        }
    }

    if (start) {
        update_button(&playButton);
        update_button(&shopButton);
        update_button(&ldbButton);

        if (ldbButton.hovered && mouseState.buttons == 1) {
            al_play_sample(pressAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            change_scene(create_ldb_scene());
            al_rest(0.2);
        }

        if (playButton.hovered && mouseState.buttons == 1) {
            al_play_sample(pressAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            change_scene(create_loading_scene());
            al_rest(0.2);
        }

        if (shopButton.hovered && mouseState.buttons == 1) {
            al_play_sample(pressAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            change_scene(create_shop_scene());
            al_rest(0.2);
        }
    }
    

}

static void draw(void) {
    // Title Text

    if (currentBg) {
        al_draw_tinted_scaled_bitmap(currentBg, al_map_rgb(255, 255, 255),
            0, 0,
            800, 800,
            0, 0,
            800, 800,
            0
        );
    }
    
    int offsetx = 32 * (star_animation_tick / (64 / 4));
    
    al_draw_tinted_scaled_bitmap(stars, al_map_rgb(255, 255, 255),
        offsetx, 0, 32, 32,
        200, 100, 100, 100, 0
    );

    al_draw_tinted_scaled_bitmap(stars, al_map_rgb(255, 255, 255),
        offsetx, 0, 32, 32,
        650, 400, 100, 100, 0
    );

    al_draw_tinted_scaled_bitmap(stars, al_map_rgb(255, 255, 255),
        offsetx, 0, 32, 32,
        100, 600, 100, 100, 0
    );


    // button
    draw_button(settingButton);
    al_draw_tinted_scaled_bitmap(settingBitmap, al_map_rgb(66, 76, 110),
        0, 0,
        800, 800,
        721, 32 + settingButton.hovered * 11,
        35, 35, 0
    );

    al_draw_tinted_scaled_bitmap(settingBitmap, al_map_rgb(255, 255, 255),
        0, 0,
        800, 800,
        721, 36 + settingButton.hovered * 11,
        35, 35, 0
    );


    if (start == 0) {
        draw_button(startButton);
        al_draw_text(
            P1_FONT, al_map_rgb(66, 76, 110),
            SCREEN_W / 2,
            675 + 23 + startButton.hovered * 3,
            ALLEGRO_ALIGN_CENTER,
            "START"
        );
        al_draw_text(
            P1_FONT, al_map_rgb(225, 225, 225),
            SCREEN_W / 2,
            675 + 23 + 3 + startButton.hovered * 3,
            ALLEGRO_ALIGN_CENTER,
            "START"
        );
    }

    if (start) {
        playColor = playButton.hovered ? al_map_rgb(255, 182, 0) : al_map_rgb(71, 42, 20);
        ldbColor = ldbButton.hovered ? al_map_rgb(255, 182, 0) : al_map_rgb(71, 42, 20);
        shopColor = shopButton.hovered ? al_map_rgb(255, 182, 0) : al_map_rgb(71, 42, 20);

        al_draw_tinted_scaled_bitmap(startMenu, al_map_rgb(255, 255, 255),
            0, 0,
            560, 300,
            SCREEN_W / 2 - 280,
            400,
            560, 300,
            0
        ); 

        al_draw_text(
            P2_FONT,
            ldbColor,
            SCREEN_W / 2,
            600 + 10,
            ALLEGRO_ALIGN_CENTER,
            "LEADERBOARD"
        );


        al_draw_text(
            P2_FONT,
            playColor,
            SCREEN_W / 2,
            400 + 50,
            ALLEGRO_ALIGN_CENTER,
            "PLAY"
        );
        al_draw_text(
            P2_FONT,
            shopColor,
            SCREEN_W / 2,
            500 + 31,
            ALLEGRO_ALIGN_CENTER,
            "SHOP"
        );
    }
   
   
   

    
}

static void destroy(void) {
    destroy_button(&settingButton);
    destroy_button(&playButton);
    destroy_button(&shopButton);
    destroy_button(&ldbButton);

    al_destroy_bitmap(currentBg);
}


Scene create_menu_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "menu";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
