#ifndef __PLAYER__
#define __PLAYER__

#include "swf.h"

typedef struct {
    int depth;
    int id;
    int tag;
    void* data;
} player_DISPLAYOBJECT;

typedef struct {
    swf* current_swf;

    int current_frame;
    int current_tag;
} player_STATUS;

void player_init(swf* _swf);
void player_process_frame();
void player_start_draw_frame();
void player_end_draw_frame();
void player_loop();
void player_quit();

#endif