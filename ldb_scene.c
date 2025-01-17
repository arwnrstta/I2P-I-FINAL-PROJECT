#include <allegro5/allegro.h>
#include <string.h>
#include "menu_scene.h"
#include "game_scene.h"
#include "setting_scene.h"
#include "ldb_scene.h"
#include "loading_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"
#include "time.h"

ldb leaderboard[50];
int entryCount;

static Button backButton;
static Button rightArrowButton;
static Button leftArrowButton;

ALLEGRO_BITMAP* squareBg;
ALLEGRO_BITMAP* ldbTitle;
ALLEGRO_BITMAP* leftArrow;
ALLEGRO_BITMAP* rightArrow;
ALLEGRO_BITMAP* coinBitmap;

ALLEGRO_BITMAP* morningBg;
ALLEGRO_BITMAP* eveningBg;
ALLEGRO_BITMAP* nightBg;
ALLEGRO_BITMAP* currentBg;

ALLEGRO_SAMPLE* pressAudio;

int firstPage = 1;
int lastPage = 0;

int start;

static void init()
{
    start = 0;
    squareBg = al_load_bitmap("Assets/submit.png");
    ldbTitle = al_load_bitmap("Assets/LEADERBOARD.png");
    leftArrow = al_load_bitmap("Assets/arrow.png");
    rightArrow = al_load_bitmap("Assets/arrow.png");
    coinBitmap = al_load_bitmap("Assets/coin_icon.png");

    morningBg = al_load_bitmap("Assets/morning_bg.png");
    eveningBg = al_load_bitmap("Assets/evening_bg.png");
    nightBg = al_load_bitmap("Assets/night_bg.png");
    currentBg = NULL;

    pressAudio = al_load_sample("Assets/audio/press.mp3");

    backButton = button_create(
        SCREEN_W / 2 - 116, 575, 
        222, 100, 
        "Assets/UI_Button.png", "Assets/UI_Button_hovered.png"
    );

    rightArrowButton = button_create(
        SCREEN_W / 2 + 160, 575, 
        100, 100, 
        "Assets/UI_SquareButton.png", "Assets/UI_SquareButton_hovered.png"
    );

    leftArrowButton = button_create(
        SCREEN_W / 2 - 260, 575, 
        100, 100, 
        "Assets/UI_SquareButton.png", "Assets/UI_SquareButton_hovered.png"
    );

    FILE* f = fopen("Assets/leaderboard.txt", "r");

    fscanf_s(f, "%d", &entryCount);

    int i = 1;
    while (i <= entryCount) {
        fscanf_s(f, "%s %d", leaderboard[i].names, sizeof(leaderboard[i].names), &leaderboard[i].points);
        i++;
    }

    for (int j = 1; j <= entryCount; j++) {
        for (int k = 1; k <= entryCount - j; k++) {
            if (leaderboard[k].points < leaderboard[k + 1].points) {
                ldb temp = leaderboard[k];
                leaderboard[k] = leaderboard[k + 1];
                leaderboard[k + 1] = temp;
            }
        }
    }

}

static void update()
{
    update_button(&backButton);
    update_button(&rightArrowButton);
    update_button(&leftArrowButton);

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

    if (mouseState.buttons && backButton.hovered == true) {
        al_play_sample(pressAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        change_scene(create_menu_scene());
        al_rest(0.2);
    }

    if (start <= 0) {
        start = 0;
        if (mouseState.buttons && rightArrowButton.hovered == true) {
            start += 5;
            al_play_sample(pressAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            al_rest(0.2);
        }
    }
    else if (start + 5 >= entryCount) {
        if (mouseState.buttons && leftArrowButton.hovered == true) {
            start -= 5;
            al_play_sample(pressAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            al_rest(0.2);
        }
    }
    else {
        if (mouseState.buttons && rightArrowButton.hovered == true) {
            if (start + 5 < entryCount) {
                start += 5;
                al_play_sample(pressAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                al_rest(0.2);
            }
            
        }
        if (mouseState.buttons && leftArrowButton.hovered == true) {
            if (start - 5 >= 0) {
                start -= 5;
                al_play_sample(pressAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                al_rest(0.2);
            }
        }
    }
}

static void draw()
{
    if (currentBg) {
        al_draw_tinted_scaled_bitmap(currentBg, al_map_rgb(255, 255, 255),
            0, 0,
            800, 800,
            0, 0,
            800, 800,
            0
        );
    }

    al_draw_tinted_scaled_bitmap(
        squareBg, al_map_rgb(255, 255, 255),
        0, 0,
        700, 700,
        80, 50,
        700, 700,
        0
    );

    al_draw_tinted_scaled_bitmap(
        ldbTitle, al_map_rgb(255, 255, 255),
        0, 0,
        800, 800,
        SCREEN_W / 2 - 200, 75,
        400, 400, 0
        );

    draw_button(backButton);

    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        575 + 28 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        SCREEN_W / 2,
        575 + 31 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );

    
    if (start+5 < entryCount) {
        draw_button(rightArrowButton);
        al_draw_tinted_scaled_bitmap(rightArrow, al_map_rgb(66, 76, 110),
            0, 0,
            800, 800,
            SCREEN_W / 2 + 190, 595 + rightArrowButton.hovered * 11, 45, 45,
            0
        ); 

        al_draw_tinted_scaled_bitmap(rightArrow, al_map_rgb(255, 255, 255),
            0, 0,
            800, 800,
            SCREEN_W / 2 + 190, 595 + 3 + rightArrowButton.hovered * 11, 45, 45,
            0
        );
    }
    if (start - 5 >= 0) {
        draw_button(leftArrowButton);
        al_draw_tinted_scaled_bitmap(leftArrow, al_map_rgb(66, 76, 110),
            0, 0,
            800, 800,
            SCREEN_W / 2 - 235, 595 + leftArrowButton.hovered * 11, 45, 45,
            1
        );

        al_draw_tinted_scaled_bitmap(leftArrow, al_map_rgb(255, 255, 255),
            0, 0,
            800, 800,
            SCREEN_W / 2 - 235, 595 + 3 + leftArrowButton.hovered * 11, 45, 45,
            1
        );

    }
    

    for (int i = 1; i <= 5 ; i++) {
        if (start + i > entryCount) {
            break;
        }
        char number[50];
        snprintf(number, sizeof(number), "%d", start+i);

        char pointStr[50];
        snprintf(pointStr, sizeof(pointStr), "%d", leaderboard[start+i].points);

        al_draw_tinted_scaled_bitmap(coinBitmap, al_map_rgb(255, 255, 255),
            0, 0,
            16, 16,
            530, 115 + (i * 75),
            50, 50, 0
       );
        al_draw_text(P2_FONT, al_map_rgb(71, 42, 20),
            125, 125 + 3 + (i * 75),
            ALLEGRO_ALIGN_LEFT,
            number
        );

        al_draw_text(P2_FONT, al_map_rgb(71, 42, 20),
            175, 125 + 3 + (i * 75),
            0,
            leaderboard[start+i].names 
        );

        al_draw_text(P2_FONT, al_map_rgb(71, 42, 20),
            670, 125 + 3 + (i * 75),
            ALLEGRO_ALIGN_RIGHT,
            pointStr
        );  
    }
}

static void destroy()
{
    destroy_button(&backButton);
    destroy_button(&rightArrowButton);
    destroy_button(&leftArrowButton);

    al_destroy_bitmap(rightArrow);
    al_destroy_bitmap(leftArrow);
    al_destroy_bitmap(ldbTitle);
    al_destroy_bitmap(coinBitmap);
}

Scene create_ldb_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "ldb";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
