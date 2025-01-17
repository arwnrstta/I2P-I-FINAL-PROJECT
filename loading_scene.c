#include <allegro5/allegro.h>
#include <string.h>
#include "menu_scene.h"
#include "loading_scene.h"
#include "game_scene.h"
#include "setting_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"

static int timer_countdown;
static ALLEGRO_BITMAP* player_bitmap;
ALLEGRO_BITMAP* levelBitmap;

int currentLevel;

static void init(void) {
    timer_countdown = 60;

    player_bitmap = al_load_bitmap("Assets/panda2.png");
    if (!player_bitmap) {
        game_abort("Failed to load player bitmap");
    }

    levelBitmap = al_load_bitmap("Assets/loading.png");

    change_bgm("None");
}

static void update(void) {
    timer_countdown--;
    if (timer_countdown == 0) {
        change_scene(create_game_scene());
    }
}

static void draw(void) {
    al_draw_scaled_bitmap(
        levelBitmap,
        0, 0,
        800, 800,
        0, 0,
        800, 800, 0
    );

    char level[50];
    snprintf(level, sizeof(level), "LEVEL %d", currentLevel);

    al_draw_text(
        TITLE_FONT,
        al_map_rgb(225, 225, 225),
        SCREEN_W / 2,
        SCREEN_H / 2 - 25,
        ALLEGRO_ALIGN_CENTER,
        level
    );

   
}

static void destroy(void) {
    al_destroy_bitmap(player_bitmap);
}


Scene create_loading_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "loading";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
