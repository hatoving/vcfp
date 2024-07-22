#include <stdio.h>

#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/clib.h>

#include "swf.h"
#include "player.h"

#define SWF_FILE_PATH "ux0:data/SWF/test.swf"

int main() {
    sceClibPrintf("[welcome to v e r y c o o l f l a s h p l a y e r]\n");
    sceClibPrintf("[vcfp] opening \"%s\"\n", SWF_FILE_PATH);

    FILE* file = fopen(SWF_FILE_PATH, "rb");
    swf _swf = read_swf(file);

    player_init(&_swf);
    player_process_frame();
    player_draw_frame();

    fclose(file);

    player_quit();

    return 0;
}