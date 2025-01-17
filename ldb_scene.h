#ifndef ldb_scene_h
#define ldb_scene_h

#include "utility.h"
#include "submit_scene.h"

Scene create_ldb_scene(void);

extern int entryCount;

typedef struct ldb {
	char names[11];
	int points;
}ldb;

#endif /* ldb_scene_h */
