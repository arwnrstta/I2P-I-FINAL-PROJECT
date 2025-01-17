#include <allegro5/allegro.h>
#include "setting_scene.h"
#include "menu_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"
#include "time.h"

static Button backButton;

static Button BGMSlider;
static Button BGMSliderHov;
static Button BGMButton;

static Button SFXSlider;
static Button SFXSliderHov;
static Button SFXButton;

ALLEGRO_BITMAP* morningBg;
ALLEGRO_BITMAP* eveningBg;
ALLEGRO_BITMAP* nightBg;
ALLEGRO_BITMAP* currentBg;

static void init(void) {
    backButton = button_create(SCREEN_W / 2 - 200, 650, 400, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");

    //BGM
    BGMSliderHov = button_create(
        225, 170, 
        480, 16, 
        "Assets/slider_hovered.png", "Assets/slider_hovered.png"
    );
    BGMSlider = button_create(
        225, 170, 
        480 * BGM_VOLUME, 16, 
        "Assets/slider.png", "Assets/slider.png"
    );
    BGMButton = button_create(
        225 + ((480 - 60) * BGM_VOLUME),
        150, 
        60, 60, 
        "Assets/UI_SquareButton.png", "Assets/UI_SquareButton.png"
    );

    //SFX
    SFXSliderHov = button_create(
        225, 260, 
        480, 16, 
        "Assets/slider_hovered.png", "Assets/slider_hovered.png"
    );
    SFXSlider = button_create(
        225, 260, 
        480 * SFX_VOLUME, 16,
        "Assets/slider.png", "Assets/slider.png"
    );
    SFXButton = button_create(
        225 + ((480 - 60) * SFX_VOLUME),
        240, 
        60, 60, 
        "Assets/UI_SquareButton.png", "Assets/UI_SquareButton.png"
    );
    
    morningBg = al_load_bitmap("Assets/morning_bg.png");
    eveningBg = al_load_bitmap("Assets/evening_bg.png");
    nightBg = al_load_bitmap("Assets/night_bg.png");
    currentBg = NULL;
}

static void update(void) {

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

    ALLEGRO_MIXER* bgmsound = al_get_default_mixer();
    al_set_mixer_gain(bgmsound, BGM_VOLUME);

    update_button(&backButton);

    if (mouseState.buttons && backButton.hovered == true) {
        change_scene(create_menu_scene());
    }

    update_button(&BGMButton);
    update_button(&SFXButton);
    
    if (mouseState.buttons && BGMButton.hovered == true) {
        BGMButton.x = mouseState.x - (BGMButton.w / 2);
        
        if (BGMButton.x < BGMSliderHov.x) {
            BGMButton.x = BGMSliderHov.x;
        }
        if (BGMButton.x > (BGMSliderHov.x + BGMSliderHov.w - BGMButton.w)) {
            BGMButton.x = BGMSliderHov.x + BGMSliderHov.w - BGMButton.w;
        }
        //BGM = 0 -> BGMButton.x = BGMsliderHov.x (225)
        //BGM = 1 -> BGMButton.x = BGMSliderHov.x + BGMSlider.w - BGMButton.w; (675)
        BGMSlider.w = BGMButton.x - BGMSliderHov.x + BGMButton.w / 2;
        BGM_VOLUME = (float)(BGMButton.x - BGMSliderHov.x) / (float)(BGMSliderHov.w - BGMButton.w);
    }
    
    if (mouseState.buttons && SFXButton.hovered == true) {
        SFXButton.x = mouseState.x - (SFXButton.w / 2);

        if (SFXButton.x < SFXSliderHov.x) {
            SFXButton.x = SFXSliderHov.x;
        }
        if (SFXButton.x > (SFXSliderHov.x + SFXSliderHov.w - SFXButton.w)) {
            SFXButton.x = SFXSliderHov.x + SFXSliderHov.w - SFXButton.w;
        }
        //SFX = 0 -> SFXButton.x = SFXSliderHov.x (225)
        //SFX = 1 -> SFXButton.x = SFXSliderHov.x + SFXSlider.w - SFXButton.w; (675)
        SFXSlider.w = SFXButton.x - SFXSliderHov.x + SFXButton.w / 2;
        SFX_VOLUME = (float)(SFXButton.x - SFXSliderHov.x) / (float)(SFXSliderHov.w - SFXButton.w);
    }

    
}

static void draw(void) {
    if (currentBg) {
        al_draw_tinted_scaled_bitmap(currentBg, al_map_rgb(255, 255, 255),
            0, 0,
            800, 800,
            0, 0,
            800, 800,
            0
        );
    }

    // button
    draw_button(backButton);
    //button text
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        650 + 28 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        650 + 31 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );
    al_draw_text(
        P1_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        20 + 28,
        ALLEGRO_ALIGN_CENTER,
        "SETTINGS"
    );
    al_draw_text(
        P1_FONT,
        al_map_rgb(255, 255, 255),
        SCREEN_W / 2,
        20 + 31,
        ALLEGRO_ALIGN_CENTER,
        "SETTINGS"
    );

    al_draw_text(
        P1_FONT,
        al_map_rgb(255, 255, 255),
        90, 160, 0,
        "BGM"
    );
    al_draw_text(
        P1_FONT,
        al_map_rgb(255, 255, 255),
        90, 250, 0,
        "SFX"
    );

    draw_button(BGMSliderHov);
    draw_button(BGMSlider);
    draw_button(BGMButton);

    draw_button(SFXSliderHov);
    draw_button(SFXSlider);
    draw_button(SFXButton);
}

static void destroy(void) {
    destroy_button(&backButton);
    destroy_button(&BGMSlider);
    destroy_button(&BGMSliderHov);
    destroy_button(&BGMButton);
    destroy_button(&SFXSlider);
    destroy_button(&SFXSliderHov);
    destroy_button(&SFXButton);
}

Scene create_setting_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "setting";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
