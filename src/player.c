#include <psp2/kernel/clib.h>

#include <SDL2/SDL.h>
#include <vitaGL.h>

#include "player.h"
#include "tag_types.h"

player_STATUS player_current_status = {0};

SDL_Window* player_window = NULL;
SDL_Renderer* player_renderer = NULL;
SDL_GLContext player_gl_context = NULL;

SDL_Texture* player_screen_tex = NULL;

swf* player_current_swf;

int player_current_tag = 0;
int player_current_frame = 0;

bool player_stop = false;

swf_RGBA player_bg_color;

player_DISPLAYOBJECT** player_defined_objects;
int player_defined_objects_size = 0;

player_DISPLAYOBJECT** player_display_list;
int player_display_list_size = 0;

void player_init(swf* _swf) {
    player_current_swf = _swf;

    vglInitExtended(4 * 1024 * 1024, 960, 544, 8 * 1024 * 1024, SCE_GXM_MULTISAMPLE_4X);
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        sceClibPrintf("[sdl2] couldn't init. error: %s\n", SDL_GetError());
    
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    player_window = SDL_CreateWindow("flash", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 960, 544, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (player_window == NULL)
        sceClibPrintf("[sdl2] couldn't create window. error: %s\n", SDL_GetError());
    else {
        player_renderer = SDL_CreateRenderer(player_window, -1, SDL_RENDERER_ACCELERATED);
        if (player_renderer == NULL)
            sceClibPrintf("[sdl2] couldn't create renderer. error: %s\n", SDL_GetError());
    }

    SDL_RenderSetLogicalSize(player_renderer, _swf->frame_size.w, _swf->frame_size.h);
    SDL_SetRenderDrawBlendMode(player_renderer, SDL_BLENDMODE_BLEND);

    player_screen_tex = SDL_CreateTexture(player_renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_TARGET, _swf->frame_size.w, _swf->frame_size.h);
}

void player_process_frame() {
    bool exit_loop = false;

    for (int i = 0; i < player_current_swf->record_count; i++)
    {
        if (exit_loop || player_stop)
            break;

        switch(player_current_swf->records[player_current_tag].tag_type) {
            case TAG_PlaceObject2: {
                /*player_display_list_size += 1;
                player_display_list = realloc(player_display_list, player_display_list_size * sizeof(player_DISPLAYOBJECT));
                if (player_defined_objects == NULL) {
                    sceClibPrintf("Failed to allocate memory\n");
                    exit(1);
                }
                
                for (int i = 0; i < player_defined_objects_size; i++)
                {
                    if (player_defined_objects[i]) {
                        player_defined_objects[player_defined_objects_size - 1] = NULL;
                    }
                }*/
                
                break;
            }
            case TAG_SetBackgroundColor: {
                sceClibPrintf("[vcfp] Setting background color\n");
                player_bg_color = ((tag_BackgroundColorTag*)player_current_swf->records[player_current_tag].tag)->rgb;
                break;
            }
            case TAG_DefineShape: {
                sceClibPrintf("[vcfp] Defining shape...\n");

                /*player_DISPLAYOBJECT* object = (player_DISPLAYOBJECT*)malloc(sizeof(player_DISPLAYOBJECT));

                object->id = ((tag_DefineShapeTag*)player_current_swf->records[player_current_tag].tag)->shape_id;
                object->tag = player_current_swf->records[player_current_tag].tag_type;
                object->data = ((tag_DefineShapeTag*)player_current_swf->records[player_current_tag].tag);

                player_defined_objects_size += 1;
                player_defined_objects = realloc(player_defined_objects, player_defined_objects_size * sizeof(player_DISPLAYOBJECT));
                if (player_defined_objects == NULL) {
                    sceClibPrintf("Failed to allocate memory\n");
                    exit(1);
                }
                player_defined_objects[player_defined_objects_size - 1] = object;*/
                break;
            }
            case TAG_ShowFrame: {
                sceClibPrintf("[vcfp] Showing frame...\n");
                exit_loop = true;
                player_stop = true;
                break;
            }
            default: {
                sceClibPrintf("[vcfp] Unsupported tag: %i\n", player_current_swf->records[player_current_tag].tag_type);
                break;
            }
        }
        player_current_tag += 1;
    }
}

void player_loop() {    
    while (1) {
        player_process_frame();

        player_start_draw_frame();


        player_end_draw_frame();
    }
}

void player_start_draw_frame() {
    SDL_SetRenderTarget(player_renderer, player_screen_tex);

    SDL_SetRenderDrawColor(player_renderer, player_bg_color.R, player_bg_color.G, player_bg_color.B, 0xFF);
    SDL_RenderClear(player_renderer);

    //SDL_SetRenderDrawColor(player_renderer, 0xFF, 0, 0, 0xFF);
    //ySDL_RenderDrawLine(renderer, 0, 0, current_swf->frame_size.w, current_swf->frame_size.h);
}

void player_end_draw_frame() {
    SDL_SetRenderTarget(player_renderer, NULL);
    
    SDL_SetRenderDrawColor(player_renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(player_renderer);

    SDL_RenderCopy(player_renderer, player_screen_tex, NULL, NULL);

    SDL_GL_SwapWindow(player_window);
    SDL_RenderPresent(player_renderer);
    //SDL_RenderSetLogicalSize(renderer, current_swf->frame_size.w, current_swf->frame_size.h);
}

void player_quit() {
    free(player_defined_objects);
    free(player_display_list);
    free(player_current_swf);

    SDL_DestroyTexture(player_screen_tex);

    SDL_GL_DeleteContext(player_gl_context);
    SDL_DestroyRenderer(player_renderer);
    SDL_DestroyWindow(player_window);
    SDL_Quit();
}