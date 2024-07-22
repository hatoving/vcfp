#ifndef __PLAYER__
#define __PLAYER__

#include "swf.h"

typedef struct {
    swf* current_swf;

    int current_frame;
    int current_tag;
} player_STATUS;

void player_init(swf* _swf);
void player_process_frame();
void player_draw_frame();
void player_quit();

#endif