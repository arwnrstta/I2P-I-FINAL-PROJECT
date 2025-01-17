#include <allegro5/allegro.h>
#include <string.h>
#include "menu_scene.h"
#include "game_scene.h"
#include "setting_scene.h"
#include "submit_scene.h"
#include "shop_scene.h"
#include "ldb_scene.h"
#include "utility.h"
#include "UI.h"
#include "game.h"
#include "player.h"

Player player;

static Button A, B, C, D, E, F;
static Button G, H, I, J, K, L;
static Button M, N, O, P, Q, R;
static Button S, T, U, V, W, X;
static Button Y, Z;

static Button clearButton;
static Button submitButton;

ALLEGRO_BITMAP* background;
ALLEGRO_BITMAP* squareBG;
int totalPoints;
int coins_obtained;
int lose;
int win;

ALLEGRO_SAMPLE* pressAudio;
ALLEGRO_SAMPLE* typeAudio;

int entryCount;
int counter = 0;
char input[11] = { '\0' };


static void init()
{
    background = al_load_bitmap("Assets/bg.png");
    squareBG = al_load_bitmap("Assets/submitscene.png");
    
    int ori_x = 130;
    int ori_y = 125;
    int x = (80 + 10);
    int y = (80 + 3);

    clearButton = button_create(170, 640, 200, 90, "Assets/UI_Button2.png", "Assets/UI_Button_hovered2.png");
    submitButton = button_create(420, 640, 200, 90, "Assets/UI_Button2.png", "Assets/UI_Button_hovered2.png");

    

    pressAudio = al_load_sample("Assets/audio/press.mp3");
    typeAudio = al_load_sample("Assets/audio/type.mp3");

    A = button_create(ori_x, ori_y, 80, 80, "Assets/ALPHABET/A.png", "Assets/ALPHABET/A_HOV.png");
    B = button_create(ori_x + x, ori_y, 80, 80, "Assets/ALPHABET/B.png", "Assets/ALPHABET/B_HOV.png");
    C = button_create(ori_x + 2*x, ori_y, 80, 80, "Assets/ALPHABET/C.png", "Assets/ALPHABET/C_HOV.png");
    D = button_create(ori_x + 3*x, ori_y, 80, 80, "Assets/ALPHABET/D.png", "Assets/ALPHABET/D_HOV.png");
    E = button_create(ori_x + 4*x, ori_y, 80, 80, "Assets/ALPHABET/E.png", "Assets/ALPHABET/E_HOV.png");
    F = button_create(ori_x + 5*x, ori_y, 80, 80, "Assets/ALPHABET/F.png", "Assets/ALPHABET/F_HOV.png");

    G = button_create(ori_x, ori_y + y, 80, 80, "Assets/ALPHABET/G.png", "Assets/ALPHABET/G_HOV.png");
    H = button_create(ori_x + ((80 + 10) * 1), ori_y + y, 80, 80, "Assets/ALPHABET/H.png", "Assets/ALPHABET/H_HOV.png");
    I = button_create(ori_x + ((80 + 10) * 2), ori_y + y, 80, 80, "Assets/ALPHABET/I.png", "Assets/ALPHABET/I_HOV.png");
    J = button_create(ori_x + ((80 + 10) * 3), ori_y + y, 80, 80, "Assets/ALPHABET/J.png", "Assets/ALPHABET/J_HOV.png");
    K = button_create(ori_x + ((80 + 10) * 4), ori_y + y, 80, 80, "Assets/ALPHABET/K.png", "Assets/ALPHABET/K_HOV.png");
    L = button_create(ori_x + ((80 + 10) * 5), ori_y + y, 80, 80, "Assets/ALPHABET/L.png", "Assets/ALPHABET/L_HOV.png");

    M = button_create(ori_x, ori_y + 2*y, 80, 80, "Assets/ALPHABET/M.png", "Assets/ALPHABET/M_HOV.png");
    N = button_create(ori_x + ((80 + 10) * 1), ori_y + 2 * y, 80, 80, "Assets/ALPHABET/N.png", "Assets/ALPHABET/N_HOV.png");
    O = button_create(ori_x + ((80 + 10) * 2), ori_y + 2 * y, 80, 80, "Assets/ALPHABET/O.png", "Assets/ALPHABET/O_HOV.png");
    P = button_create(ori_x + ((80 + 10) * 3), ori_y + 2 * y, 80, 80, "Assets/ALPHABET/P.png", "Assets/ALPHABET/P_HOV.png");
    Q = button_create(ori_x + ((80 + 10) * 4), ori_y + 2 * y, 80, 80, "Assets/ALPHABET/Q.png", "Assets/ALPHABET/Q_HOV.png");
    R = button_create(ori_x + ((80 + 10) * 5), ori_y + 2 * y, 80, 80, "Assets/ALPHABET/R.png", "Assets/ALPHABET/R_HOV.png");

    S = button_create(ori_x, ori_y + 3 * y, 80, 80, "Assets/ALPHABET/S.png", "Assets/ALPHABET/S_HOV.png");
    T = button_create(ori_x + ((80 + 10) * 1), ori_y + 3 * y, 80, 80, "Assets/ALPHABET/T.png", "Assets/ALPHABET/T_HOV.png");
    U = button_create(ori_x + ((80 + 10) * 2), ori_y + 3 * y, 80, 80, "Assets/ALPHABET/U.png", "Assets/ALPHABET/U_HOV.png");
    V = button_create(ori_x + ((80 + 10) * 3), ori_y + 3 * y, 80, 80, "Assets/ALPHABET/V.png", "Assets/ALPHABET/V_HOV.png");
    W = button_create(ori_x + ((80 + 10) * 4), ori_y + 3 * y, 80, 80, "Assets/ALPHABET/W.png", "Assets/ALPHABET/W_HOV.png");
    X = button_create(ori_x + ((80 + 10) * 5), ori_y + 3 * y, 80, 80, "Assets/ALPHABET/X.png", "Assets/ALPHABET/X_HOV.png");

    Y = button_create(ori_x + ((80 + 10) * 2), ori_y + 4 * y, 80, 80, "Assets/ALPHABET/Y.png", "Assets/ALPHABET/Y_HOV.png");
    Z = button_create(ori_x + ((80 + 10) * 3), ori_y + 4 * y, 80, 80, "Assets/ALPHABET/Z.png", "Assets/ALPHABET/Z_HOV.png");
}

static void update()
{
    update_button(&clearButton);
    update_button(&submitButton);

    update_button(&A);
    update_button(&B);
    update_button(&C);
    update_button(&D);
    update_button(&E);
    update_button(&F);

    update_button(&G);
    update_button(&H);
    update_button(&I);
    update_button(&J);
    update_button(&K);
    update_button(&L);

    update_button(&M);
    update_button(&N);
    update_button(&O);
    update_button(&P);
    update_button(&Q);
    update_button(&R);

    update_button(&S);
    update_button(&T);
    update_button(&U);
    update_button(&V);
    update_button(&W);
    update_button(&X);

    update_button(&Y);
    update_button(&Z);


    if (counter > 0 && clearButton.hovered && mouseState.buttons == 1) {
        for (int i = 0; i < counter; i++) {
            input[i] = '\0';
        }
        counter = 0;
        al_play_sample(pressAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }

    if (counter > 0 && submitButton.hovered && mouseState.buttons == 1) {
        FILE* file = fopen("Assets/leaderboard.txt", "r+");
        fscanf_s(file, "%d", &entryCount);
        rewind(file);

        fprintf(file, "%d\n", entryCount + 1);

        fseek(file, 0, SEEK_END);
        fprintf(file, "%s %d\n", input, totalPoints);
        fclose(file);

        for (int i = 0; i < counter; i++) {
            input[i] = '\0';
        }
        counter = 0;

        al_play_sample(pressAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        change_scene(create_ldb_scene());
        al_rest(0.2);
    }

    //BUTTON UPDATE HELP
    if (counter < 10 && A.hovered && mouseState.buttons == 1) {
        input[counter++] = 'A';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && B.hovered && mouseState.buttons == 1) {
        input[counter++] = 'B';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && C.hovered && mouseState.buttons == 1) {
        input[counter++] = 'C';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && D.hovered && mouseState.buttons == 1) {
        input[counter++] = 'D';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && E.hovered && mouseState.buttons == 1) {
        input[counter++] = 'E';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && F.hovered && mouseState.buttons == 1) {
        input[counter++] = 'F';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }

    if (counter < 10 && G.hovered && mouseState.buttons == 1) {
        input[counter++] = 'G';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && H.hovered && mouseState.buttons == 1) {
        input[counter++] = 'H';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && I.hovered && mouseState.buttons == 1) {
        input[counter++] = 'I';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && J.hovered && mouseState.buttons == 1) {
        input[counter++] = 'J';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && K.hovered && mouseState.buttons == 1) {
        input[counter++] = 'K';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && L.hovered && mouseState.buttons == 1) {
        input[counter++] = 'L';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }

    if (counter < 10 && M.hovered && mouseState.buttons == 1) {
        input[counter++] = 'M';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && N.hovered && mouseState.buttons == 1) {
        input[counter++] = 'N';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && O.hovered && mouseState.buttons == 1) {
        input[counter++] = 'O';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && P.hovered && mouseState.buttons == 1) {
        input[counter++] = 'P';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && Q.hovered && mouseState.buttons == 1) {
        input[counter++] = 'Q';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && R.hovered && mouseState.buttons == 1) {
        input[counter++] = 'R';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }

    if (counter < 10 && S.hovered && mouseState.buttons == 1) {
        input[counter++] = 'S';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && T.hovered && mouseState.buttons == 1) {
        input[counter++] = 'T';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && U.hovered && mouseState.buttons == 1) {
        input[counter++] = 'U';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && V.hovered && mouseState.buttons == 1) {
        input[counter++] = 'V';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && W.hovered && mouseState.buttons == 1) {
        input[counter++] = 'W';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && X.hovered && mouseState.buttons == 1) {
        input[counter++] = 'X';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }

    if (counter < 10 && Y.hovered && mouseState.buttons == 1) {
        input[counter++] = 'Y';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }
    if (counter < 10 && Z.hovered && mouseState.buttons == 1) {
        input[counter++] = 'Z';
        al_play_sample(typeAudio, SFX_VOLUME, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        al_rest(0.2);
    }

}

static void draw()
{
    al_draw_tinted_scaled_bitmap(background, al_map_rgb(255, 255, 255),
        0, 0,
        800, 800,
        0, 0,
        800, 800, 0
    );

    al_draw_tinted_scaled_bitmap(squareBG, al_map_rgb(255, 255, 255),
        0, 0,
        630, 718,
        80, 40,
        630, 718,
        0
    );
    
    char message[50];
    snprintf(message, sizeof(message), "YOUR SCORE IS %d", totalPoints);
    al_draw_text(
        P2_FONT, al_map_rgb(66, 76, 110),
        SCREEN_W / 2, 70,
        ALLEGRO_ALIGN_CENTER,
        message
    );
    al_draw_text(
        P2_FONT, al_map_rgb(255, 255, 255),
        SCREEN_W / 2, 70+3,
        ALLEGRO_ALIGN_CENTER,
        message
    );

    draw_button(clearButton);
    draw_button(submitButton);

    al_draw_text(
        P2_FONT, al_map_rgb(66, 77, 110),
        210, 665 + clearButton.hovered*11,
        0,
        "CLEAR"
    );
    al_draw_text(
        P2_FONT, al_map_rgb(255, 255, 255),
        210, 665 + 3 + clearButton.hovered*11,
        0,
        "CLEAR"
    );

    al_draw_text(
        P2_FONT, al_map_rgb(66, 77, 110),
        455, 665 + submitButton.hovered*11,
        0,
        "SUBMIT"
    );
    al_draw_text(
        P2_FONT, al_map_rgb(255, 255, 255),
        455, 665 + 3 + submitButton.hovered * 11,
        0,
        "SUBMIT"
    );

    al_draw_text(
        P2_FONT, al_map_rgb(66, 77, 110),
        SCREEN_W/2, 560,
        ALLEGRO_ALIGN_CENTRE,
        input
    );
    al_draw_text(
        P2_FONT, al_map_rgb(255, 255, 255),
        SCREEN_W/2, 560+3,
        ALLEGRO_ALIGN_CENTRE,
        input
    );


    //ALPHABET BUTTONS LOLLL
    draw_button(A);
    draw_button(B);
    draw_button(C);
    draw_button(D);
    draw_button(E);
    draw_button(F);

    draw_button(G);
    draw_button(H);
    draw_button(I);
    draw_button(J);
    draw_button(K);
    draw_button(L);

    draw_button(M);
    draw_button(N);
    draw_button(O);
    draw_button(P);
    draw_button(Q);
    draw_button(R);

    draw_button(S);
    draw_button(T);
    draw_button(U);
    draw_button(V);
    draw_button(W);
    draw_button(X);

    draw_button(Y);
    draw_button(Z);
}

static void destroy()
{

}

Scene create_submit_scene(void) {
    Scene scene;
    memset(&scene, 0, sizeof(Scene));

    scene.name = "submit";
    scene.init = &init;
    scene.draw = &draw;
    scene.update = &update;
    scene.destroy = &destroy;

    return scene;
}
