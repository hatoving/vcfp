#ifndef __SWF_TYPES__
#define __SWF_TYPES__

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "tag_types.h"

typedef enum {
    TAG_End,
    TAG_ShowFrame,
    TAG_DefineShape,
    TAG_SetBackgroundColor = 9,
    TAG_PlaceObject2 = 26,
} swf_TAGTYPE;

typedef struct {
    uint8_t R;
    uint8_t G;
    uint8_t B;
} swf_RGB;
swf_RGB read_swf_RGB(FILE* file);

typedef struct {
    uint16_t tag_type;
    uint32_t length;
    long offset;

    tag_Collection collection;
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
} swf_MATRX;

#endif