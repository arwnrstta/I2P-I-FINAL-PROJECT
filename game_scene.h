#ifndef game_scene_h
#define game_scene_h

#include "utility.h"

extern int win;
extern int lose;

extern int weaponStat; //gun
extern int sniperStat; 

extern int currentHealth;

Scene create_game_scene(void);
Scene create_win_scene(void);
Scene create_lose_scene(void);


#endif /* game_scene_h */
