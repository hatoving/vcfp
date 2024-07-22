#include <psp2/kernel/clib.h>
#include <SDL2/SDL.h>

#include "player.h"

player_STATUS current_player_status = {0};

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Texture* screen_tex = NULL;

swf* current_swf;

int current_tag = 0;
int current_frame = 0;

swf_RGB background_color;

void player_init(swf* _swf) {
    current_swf = _swf;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        sceClibPrintf("[sdl2] couldn't init. error: %s\n", SDL_GetError());

    window = SDL_CreateWindow("flash", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 544, SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == NULL)
        sceClibPrintf("[sdl2] couldn't create window. error: %s\n", SDL_GetError());
    else {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == NULL)
            sceClibPrintf("[sdl2] couldn't create renderer. error: %s\n", SDL_GetError());
    }

    SDL_RenderSetLogicalSize(renderer, _swf->frame_size.w, _swf->frame_size.h);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    screen_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_TARGET, _swf->frame_size.w, _swf->frame_size.h);
}

void player_process_frame() {
    bool exit_loop = false;

    for (int i = 0; i < current_swf->record_count; i++)
    {
        if (exit_loop)
            break;

        switch(current_swf->records[current_tag].tag_type) {
            case 9: {
                sceClibPrintf("[vcfp] Setting background color\n");
                background_color = current_swf->records[current_tag].collection.background_tag.rgb;
                break;
            }
            case 2: {
                
                break;
            }
            case 1: {
                sceClibPrintf("[vcfp] Showing frame...\n");
                exit_loop = true;
                break;
            }
            default: {
                sceClibPrintf("[vcfp] Unsupported tag: %i\n", current_swf->records[current_tag].tag_type);
                break;
            }
        }
        current_tag += 1;
    }
}

void player_draw_frame() {
    while (1) {
        SDL_SetRenderTarget(renderer, screen_tex);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, background_color.R, background_color.G, background_color.B, 0xFF);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
        SDL_RenderDrawLine(renderer, 0, 0, current_swf->frame_size.w, current_swf->frame_size.h);

        SDL_SetRenderTarget(renderer, NULL);
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, screen_tex, NULL, NULL);

        SDL_RenderPresent(renderer);
        //SDL_RenderSetLogicalSize(renderer, current_swf->frame_size.w, current_swf->frame_size.h);
    }
}

void player_quit() {
    SDL_DestroyTexture(screen_tex);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}