#ifndef __SWF_TYPES__
#define __SWF_TYPES__

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t R;
    uint8_t G;
    uint8_t B;
    uint8_t A;
} swf_RGBA;
swf_RGBA read_swf_RGB(FILE* file);

typedef struct {
    uint16_t tag_type;
    uint32_t length;
    long offset;

    void* tag;
} swf_RECORDHEADER;
swf_RECORDHEADER read_swf_RECORDHEADER(FILE* file);

typedef struct {
    int8_t bits;
    int x;
    int y;
    int w;
    int h;
} swf_RECT;
swf_RECT read_swf_RECT(FILE* file);

typedef struct {
    bool has_scale;
    bool has_rotate;

    float scale_x;
    float scale_y;
    float rotate_skew_0;
    float rotate_skew_1;

    float x;
    float y;
} swf_MATRIX;

#endif