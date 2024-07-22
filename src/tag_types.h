#ifndef __TAG_TYPES__
#define __TAG_TYPES__

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "swf_types.h"

typedef struct {
    swf_RGB rgb;
} tag_BackgroundColorTag;

#pragma region SHAPE STRUCTURE

typedef struct {
    uint8_t type;
    swf_RGB color;
    swf_MATRX gradient;
} swf_FILESTYLE;

typedef struct {
    uint8_t fill_style_count;

} swf_FILESTYLEARRAY;

typedef struct {
    uint16_t shape_id;
    swf_RECT shape_bounds;

} tag_DefineShapeTag;
void tag_draw_DefineShape(tag_DefineShapeTag tag);

#pragma endregion

typedef struct {
    tag_BackgroundColorTag background_tag;
    tag_DefineShapeTag shape_tag;
} tag_Collection;

#endif