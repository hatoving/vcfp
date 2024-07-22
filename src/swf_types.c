#include "swf_types.h"
#include "bit_util.h"

#include <math.h>
#include <psp2/kernel/clib.h>

swf_RECT read_swf_RECT(FILE* file) {
    swf_RECT rect;

    bit_op_flag = 0;

    rect.bits = read_ubits(file, 5);

    int x = read_sbits(file, rect.bits);
    int w = read_sbits(file, rect.bits);
    int y = read_sbits(file, rect.bits);
    int h = read_sbits(file, rect.bits);

    rect.x = x;
    rect.y = y;
    rect.w = floor((w - x) * 0.0666666667);
    rect.h = floor((h - y) * 0.0666666667);
    
    return rect;
}

swf_RGB read_swf_RGB(FILE* file) {
    swf_RGB rgb;

    fread(&rgb.R, 1, 1, file);
    fread(&rgb.G, 1, 1, file);
    fread(&rgb.B, 1, 1, file);

    return rgb;
}

swf_RECORDHEADER read_swf_RECORDHEADER(FILE* file) {
    swf_RECORDHEADER record;

    //long pos = ftell(file);

    uint16_t tagcode_and_length;
    fread(&tagcode_and_length, 2, 1, file);
    
    record.tag_type = tagcode_and_length >> 6;
    record.length = tagcode_and_length & 0x3F;

    if (record.length == 0x3F)
        fread(&record.length, 4, 1, file);

    record.offset = ftell(file);

    //sceClibPrintf("TAG TYPE: %i\n", record.tag_type);
    return record;
}