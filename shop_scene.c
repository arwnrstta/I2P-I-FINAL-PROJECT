#include <allegro5/allegro.h>
#include "shop_scene.h"
#include "menu_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"
#include "time.h"

static Button backButton;
static Button rightArrowButton;
static Button leftArrowButton;

static Button buySniperButton;
static Button useGunButton;
static Button useSniperButton;

static Button buyAppleButton;
static Button buyBananaButton;

ALLEGRO_BITMAP* sniperBitmap;
ALLEGRO_BITMAP* gunBitmap;
ALLEGRO_BITMAP* appleBitmap;
ALLEGRO_BITMAP* bananaBitmap;

ALLEGRO_BITMAP* square;
ALLEGRO_BITMAP* arrow;
ALLEGRO_BITMAP* usedButton;
ALLEGRO_BITMAP* weaponTitle;
ALLEGRO_BITMAP* boosterTitle;
ALLEGRO_BITMAP* coinImage;

ALLEGRO_BITMAP* morningBg;
ALLEGRO_BITMAP* eveningBg;
ALLEGRO_BITMAP* nightBg;
ALLEGRO_BITMAP* currentBg;

ALLEGRO_SAMPLE* purchaseAudio;
ALLEGRO_SAMPLE* pressAudio;
ALLEGRO_SAMPLE* equipAudio;

int gunPrice = 0;
int sniperPrice = 5;
int applePrice = 1;
int bananaPrice = 1;

int inventory1 = 1; //current, no left button
int inventory2 = 0; //next, no right button
int weaponStat = 1, sniperStat = 0;
int appleStat, bananaStat;

int coins_obtained;
int totalCoins;

static void init() {
    inventory1 = 1;
    inventory2 = 0;
    
    appleStat = 0;
    bananaStat = 0;

    backButton = button_create(SCREEN_W / 2 - 111, 600, 222, 100, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    rightArrowButton = button_create(SCREEN_W / 2 + 160, 600, 100, 100, "Assets/UI_SquareButton.png", "Assets/UI_SquareButton_hovered.png");
    leftArrowButton = button_create(SCREEN_W / 2 - 260, 600, 100, 100, "Assets/UI_SquareButton.png", "Assets/UI_SquareButton_hovered.png");

    usedButton = al_load_bitmap("Assets/used_button.png");

    sniperBitmap = al_load_bitmap("Assets/sniper.png");
    gunBitmap = al_load_bitmap("Assets/guns.png");
    appleBitmap = al_load_bitmap("Assets/apple_shop.png");
    bananaBitmap = al_load_bitmap("Assets/banana_shop.png");

    arrow = al_load_bitmap("Assets/arrow.png");
    coinImage = al_load_bitmap("Assets/coin_icon.png");

    weaponTitle = al_load_bitmap("Assets/weapon_title.png");
    boosterTitle = al_load_bitmap("Assets/booster_title.png");

    purchaseAudio = al_load_sample("Assets/audio/buy.mp3");
    pressAudio = al_load_sample("Assets/audio/press.mp3");
    equipAudio = al_load_sample("Assets/audio/equip.mp3");

    buySniperButton = button_create(460, 420, 166, 75, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    useGunButton = button_create(160, 420, 166, 75, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    useSniperButton = button_create(460, 420, 166, 75, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");

    buyAppleButton = button_create(160, 420, 166, 75, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");
    buyBananaButton = button_create(460, 420, 166, 75, "Assets/UI_Button.png", "Assets/UI_Button_hovered.png");

    square = al_load_bitmap("Assets/square.png");
    morningBg = al_load_bitmap("Assets/morning_bg.png");
    eveningBg = al_load_bitmap("Assets/evening_bg.png");
    nightBg = al_load_bitmap("Assets/night_bg.png");
    currentBg = NULL;
}

static void update() {
    update_button(&backButton);
    update_button(&rightArrowButton);
    update_button(&leftArrowButton);
    update_button(&buySniperButton);
    update_button(&buyAppleButton);
    update_button(&buyBananaButton);
    update_button(&useSniperButton);
    update_button(&useGunButton);

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


    if (inventory1) {
        if (mouseState.buttons && buySniperButton.hovered == true) { //BUY SNIPER AND USE IT
            if (totalCoins >= sniperPrice) {
                totalCoins -= sniperPrice;
                sniperPrice = 0;
                sniperStat = 1;
                weaponStat = 0;
                al_play_sample(purchaseAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
        }

        if (weaponStat && sniperPrice == 0) {
            if (mouseState.buttons && useSniperButton.hovered == true) { //SWITCH TO SNIPER, ALREADY BOUGHT
                sniperStat = 1;
                weaponStat = 0;
                al_play_sample(equipAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
        }

        if (sniperStat && gunPrice == 0) { //SWITCH TO GUN
            if (mouseState.buttons && useGunButton.hovered == true) {
                weaponStat = 1;
                sniperStat = 0;
                al_play_sample(equipAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
        }
    }
   
    if (inventory2) {
        if (appleStat == 0) {
            if (mouseState.buttons && buyAppleButton.hovered == true && applePrice != 0) {
                if (totalCoins >= applePrice) {
                    totalCoins -= applePrice;
                    applePrice = 0;
                    appleStat = 1;
                    al_play_sample(purchaseAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
            }
        }

        if (bananaStat == 0) {
            if (mouseState.buttons && buyBananaButton.hovered == true && bananaPrice != 0) {
                if (totalCoins >= bananaPrice) {
                    totalCoins -= bananaPrice;
                    bananaPrice = 0;
                    bananaStat = 1;
                    al_play_sample(purchaseAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                }

            }

        }

    }
    

    if (mouseState.buttons && rightArrowButton.hovered == true) {
        inventory1 = 0;
        inventory2 = 1; 
        al_play_sample(pressAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }

    if (mouseState.buttons && leftArrowButton.hovered == true) {
        inventory1 = 1;
        inventory2 = 0;
        al_play_sample(pressAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    
}

static void draw() {
    //Background
   
    if (currentBg){
        al_draw_tinted_scaled_bitmap(currentBg, al_map_rgb(255, 255, 255),
            0, 0,
            800, 800,
            0, 0,
            800, 800,
            0
        );
    }
    

    //Total Coins
    al_draw_tinted_scaled_bitmap(coinImage, al_map_rgb(255, 255, 255),
        0, 0, 16, 16, // source image x, y, width, height
        10, 10, TILE_SIZE, TILE_SIZE, // destiny x, y, width, height
        0 // Flip or not
    );

    char coin[100];
    snprintf(coin, sizeof(coin), "%02d", totalCoins);

    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        77, 27, 0,
        coin
    );

    
    //Square Background
    al_draw_tinted_scaled_bitmap(square, al_map_rgb(255, 255, 255), 
        0, 0, 680, 680, // source image x, y, width, height
        SCREEN_W/2 - 330, SCREEN_H/2 - 270, 676, 676, // destiny x, y, width, height
        0 // Flip or not
    );

    draw_button(backButton);

    //button text
    al_draw_text(
        P2_FONT,
        al_map_rgb(66, 76, 110),
        SCREEN_W / 2,
        600 + 28 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );
    al_draw_text(
        P2_FONT,
        al_map_rgb(255, 255, 255),
        SCREEN_W / 2,
        600 + 31 + backButton.hovered * 11,
        ALLEGRO_ALIGN_CENTER,
        "BACK"
    );

    //INVENTORY 1
    if (inventory1) {
        //Arrow Button
        draw_button(rightArrowButton);

        al_draw_tinted_scaled_bitmap(arrow, al_map_rgb(66, 76, 110),
            0, 0, 800, 800, // source image x, y, width, height
            SCREEN_W / 2 + 190, 620 + rightArrowButton.hovered * 11, 45, 45, // destiny x, y, width, height
            0 // Flip or not
        );
        al_draw_tinted_scaled_bitmap(arrow, al_map_rgb(255, 255, 255),
            0, 0, 800, 800, // source image x, y, width, height
            SCREEN_W / 2 + 190, 623 + rightArrowButton.hovered * 11, 45, 45, // destiny x, y, width, height
            0 // Flip or not
        );
        //WEAPONS
        al_draw_tinted_scaled_bitmap(weaponTitle, al_map_rgb(255, 255, 255),
            0, 0, 800, 800,
            SCREEN_W/2 - 150, 45, 280, 280,
            0
        );
        //Gun
        al_draw_tinted_scaled_bitmap(gunBitmap, al_map_rgb(255, 255, 255),
            0, 0, 32, 32, // source image x, y, width, height
            160, 230, TILE_SIZE * 3, TILE_SIZE * 3, // destiny x, y, width, height
            0 // Flip or not
        );
        //Sniper
        al_draw_tinted_scaled_bitmap(sniperBitmap, al_map_rgb(255, 255, 255),
            0, 0, 32, 32, // source image x, y, width, height
            450, 230, TILE_SIZE * 3, TILE_SIZE * 3, // destiny x, y, width, height
            0 // Flip or not
        );

        if (weaponStat) { //USED BUTTON FOR GUN
            al_draw_tinted_scaled_bitmap(usedButton, al_map_rgb(255, 255, 255),
                0, 0, 800, 800,
                160, 370, 166, 166,
                0
            );
            al_draw_text(
                P2_FONT,
                al_map_rgb(66, 76, 110),
                195,
                442,
                0,
                "USED"
            );
            al_draw_text(
                P2_FONT,
                al_map_rgb(255, 255, 255),
                195,
                445,
                0,
                "USED"
            );

            if (sniperPrice == 0) { //if the sniper has been bought
                draw_button(useSniperButton); //USE
                al_draw_text(
                    P2_FONT,
                    al_map_rgb(66, 76, 110),
                    475,
                    437 + useSniperButton.hovered*11,
                    0,
                    "SWITCH"
                );
                al_draw_text(
                    P2_FONT,
                    al_map_rgb(255, 255, 255),
                    475,
                    440 + useSniperButton.hovered*11,
                    0,
                    "SWITCH"
                );
            }
            else {
                //BUY
                draw_button(buySniperButton);
                al_draw_tinted_scaled_bitmap(coinImage, al_map_rgb(255, 255, 255),
                    0, 0, 16, 16, // source image x, y, width, height
                    490, 427 + buySniperButton.hovered * 11, 50, 50, // destiny x, y, width, height
                    0 // Flip or not
                );

                char sniperArr[100];

                snprintf(sniperArr, sizeof(sniperArr), "%02d", sniperPrice);
                al_draw_text(
                    P2_FONT,
                    al_map_rgb(66, 76, 110),
                    540,
                    437 + buySniperButton.hovered * 11,
                    0,
                    sniperArr
                );
                al_draw_text(
                    P2_FONT,
                    al_map_rgb(255, 255, 255),
                    540,
                    440 + buySniperButton.hovered * 11,
                    0,
                    sniperArr
                );
            } 
        }
        else if (sniperStat) {
            al_draw_tinted_scaled_bitmap(usedButton, al_map_rgb(255, 255, 255),
                0, 0, 800, 800,
                460, 370, 166, 166,
                0
            );
            al_draw_text(
                P2_FONT,
                al_map_rgb(66, 76, 110),
                495,
                442,
                0,
                "USED"
            );
            al_draw_text(
                P2_FONT,
                al_map_rgb(255, 255, 255),
                495,
                445,
                0,
                "USED"
            );
            if (gunPrice == 0) {
                draw_button(useGunButton);
                al_draw_text(
                    P2_FONT,
                    al_map_rgb(66, 76, 110),
                    175,
                    437 + useGunButton.hovered*11,
                    0,
                    "SWITCH"
                );
                al_draw_text(
                    P2_FONT,
                    al_map_rgb(255, 255, 255),
                    175,
                    440 + useGunButton.hovered*11,
                    0,
                    "SWITCH"
                );
            }
        }
        
    }

    //INVENTORY 2
    if (inventory2) {
        draw_button(leftArrowButton);

        al_draw_tinted_scaled_bitmap(arrow, al_map_rgb(66, 76, 110),
            0, 0, 800, 800, // source image x, y, width, height
            SCREEN_W / 2 - 235, 620 + leftArrowButton.hovered * 11, 45, 45, // destiny x, y, width, height
            1 // Flip or not
        );
        al_draw_tinted_scaled_bitmap(arrow, al_map_rgb(255, 255, 255),
            0, 0, 800, 800, // source image x, y, width, height
            SCREEN_W / 2 - 235, 623 + leftArrowButton.hovered * 11, 45, 45, // destiny x, y, width, height
            1 // Flip or not
        );

        //BOOSTER TITLE
        al_draw_tinted_scaled_bitmap(boosterTitle, al_map_rgb(255, 255, 255),
            0, 0, 800, 800,
            SCREEN_W / 2 - 155, 30, 330, 330,
            0 
        );
        
        //APPLE
        al_draw_tinted_scaled_bitmap(appleBitmap, al_map_rgb(255, 255, 255),
            0, 0, 800, 800,
            180, 270, TILE_SIZE * 2, TILE_SIZE * 2,
            0
        );

        //BANANA
        al_draw_tinted_scaled_bitmap(bananaBitmap, al_map_rgb(255, 255, 255),
            0, 0, 800, 800,
            450, 270, TILE_SIZE * 2, TILE_SIZE * 2,
            0
        );

        if (applePrice > 0) {
            draw_button(buyAppleButton);
            al_draw_tinted_scaled_bitmap(coinImage, al_map_rgb(255, 255, 255),
            0, 0, 16, 16, // source image x, y, width, height
            190, 427 + buyAppleButton.hovered * 11, 50, 50, // destiny x, y, width, height
            0 // Flip or not
            );

            char appleArr[100];

            snprintf(appleArr, sizeof(appleArr), "%02d", applePrice);
            al_draw_text(
                P2_FONT,
                al_map_rgb(66, 76, 110),
                240,
                437 + buyAppleButton.hovered * 11,
                0,
                appleArr
            );
            al_draw_text(
                P2_FONT,
                al_map_rgb(255, 255, 255),
                240,
                440 + buyAppleButton.hovered * 11,
                0,
                appleArr
            );
        }
        else if (applePrice == 0){
            al_draw_tinted_scaled_bitmap(usedButton, al_map_rgb(255, 255, 255),
                0, 0, 800, 800,
                160, 370, 166, 166,
                0
            );
            al_draw_text(
                P2_FONT,
                al_map_rgb(66, 76, 110),
                195,
                442,
                0,
                "USED"
            );
            al_draw_text(
                P2_FONT,
                al_map_rgb(255, 255, 255),
                195,
                445,
                0,
                "USED"
            );
        }

        if (bananaPrice > 0) {
            draw_button(buyBananaButton);
            al_draw_tinted_scaled_bitmap(coinImage, al_map_rgb(255, 255, 255),
                0, 0, 16, 16, // source image x, y, width, height
                490, 427 + buyBananaButton.hovered * 11, 50, 50, // destiny x, y, width, height
                0 // Flip or not
            );

            char bananaArr[100];

            snprintf(bananaArr, sizeof(bananaArr), "%02d", bananaPrice);
            al_draw_text(
                P2_FONT,
                al_map_rgb(66, 76, 110),
                540,
                437 + buyBananaButton.hovered * 11,
                0,
                bananaArr
            );
            al_draw_text(
                P2_FONT,
                al_map_rgb(255, 255, 255),
                540,
                440 + buyBananaButton.hovered * 11,
                0,
                bananaArr
            );
        }
        else if (bananaPrice == 0) {
            al_draw_tinted_scaled_bitmap(usedButton, al_map_rgb(255, 255, 255),
                0, 0, 800, 800,
                460, 370, 166, 166,
                0
            );
            al_draw_text(
                P2_FONT,
                al_map_rgb(66, 76, 110),
                495,
                442,
                0,
                "USED"
            );
            al_draw_text(
                P2_FONT,
                al_map_rgb(255, 255, 255),
                495,
                445,
                0,
                "USED"
            );
        }
       
    }
    
}

static void destroy() {
    destroy_button(&backButton);
    destroy_button(&leftArrowButton);
    destroy_button(&rightArrowButton);

    destroy_button(&buySniperButton);
    destroy_button(&useSniperButton);
    destroy_button(&useGunButton);
    destroy_button(&buyAppleButton);
    destroy_button(&buyBananaButton);

    al_destroy_bitmap(usedButton);

    al_destroy_bitmap(gunBitmap);
    al_destroy_bitmap(sniperBitmap);
    al_destroy_bitmap(appleBitmap);
    al_destroy_bitmap(bananaBitmap);
    al_destroy_bitmap(weaponTitle);
    al_destroy_bitmap(boosterTitle);
}

Scene create_shop_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "shop";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
