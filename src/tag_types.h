#ifndef __TAG_TYPES__
#define __TAG_TYPES__

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>
#include "swf_types.h"

typedef enum {
    TAG_End,
    TAG_ShowFrame,
    TAG_DefineShape,
    TAG_SetBackgroundColor = 9,
    TAG_PlaceObject2 = 26,
    TAG_DefineEditText = 37
} tag_TYPE;

typedef struct {
    swf_RGBA rgb;
} tag_BackgroundColorTag;

typedef struct {
    bool has_clip_actions;
    bool has_clip_depth;
    bool has_name;
    bool has_ratio;
    bool has_color_transform;
    bool has_matrix;
    bool has_character;
    bool has_move;

    uint16_t depth;
    uint16_t char_id;

    swf_MATRIX transform;
} tag_ShowObjectTag;

#pragma region SHAPES

typedef struct {
    uint8_t type;
    swf_RGBA color;
    swf_MATRIX gradient_matrix;
} tag_ShapeFillStyle;

typedef struct {
    uint16_t shape_id;
    swf_RECT shape_bounds;
} tag_DefineShapeTag;
tag_DefineShapeTag* tag_read_DefineShape(FILE* file, swf_RECORDHEADER header);
void tag_draw_DefineShape(tag_DefineShapeTag* tag);

typedef struct {
    uint16_t shape_id;
    swf_RECT shape_bounds;
} tag_DefineShapeTag2;
tag_DefineShapeTag2* tag_read_DefineShape2(FILE* file, swf_RECORDHEADER header);
void tag_draw_DefineShape2(SDL_Renderer* renderer, tag_DefineShapeTag2* tag);

#pragma endregion

typedef struct {
    uint16_t char_id;

    bool has_text;
    bool word_wrap;
    bool multiline;
    bool password;
    bool readonly;
    bool has_text_color;
    bool has_max_length;
    bool has_font;
    bool has_font_class;
    bool auto_size;
    bool has_layout;
    bool no_select;
    bool border;
    bool was_static;
    bool html;
    bool use_outlines;
    
    uint16_t font_id;
    char* font_class;
    uint16_t font_height;
    swf_RGBA text_color;
    uint16_t max_length;
    uint8_t align;
    uint16_t left_margin;
    uint16_t right_margin;
    uint16_t indent;
    int16_t leading;
    char* var_name;

    char* initial_text;
} tag_DefineTextEdit;

#endif