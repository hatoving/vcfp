#include "tag_types.h"

#include <stdio.h>
#include <SDL2/SDL.h>

#pragma region SHAPES

tag_DefineShapeTag* tag_read_DefineShape(FILE* file, swf_RECORDHEADER header) {
    tag_DefineShapeTag tag;

    fread(&tag.shape_id, sizeof(uint16_t), 1, file);
    tag.shape_bounds = read_swf_RECT(file);

    fseek(file, header.offset + header.length, SEEK_SET);
    return &tag;
}

void draw_tag_DefineShape(tag_DefineShapeTag* tag) {
    
}

tag_DefineShapeTag2* read_tag_DefineShape2(FILE* file, swf_RECORDHEADER header) {
    tag_DefineShapeTag2 tag;

    fread(&tag.shape_id, sizeof(uint16_t), 1, file);
    tag.shape_bounds = read_swf_RECT(file);

    fseek(file, header.offset + header.length, SEEK_SET);
    return &tag;
}

void draw_tag_DefineShape2(SDL_Renderer* renderer, tag_DefineShapeTag2* tag) {
    SDL_Rect rect = {tag->shape_bounds.x, tag->shape_bounds.y, tag->shape_bounds.w, tag->shape_bounds.h};
    SDL_RenderDrawRect(renderer, tag);
}
 
#pragma endregion



