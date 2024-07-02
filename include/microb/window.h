//
// Created by alinarezrangel on 25/05/24.
//

#ifndef MICROB_WINDOW_H
#define MICROB_WINDOW_H

#include <lua.h>
#include <SDL.h>
#include <stdbool.h>

void microb_global_init(void);
void microb_global_uninit(void);

typedef struct microb_window
{
    SDL_Window *backing_window;
    SDL_Renderer *renderer;
    bool freed;

    int component_list_ref;

    struct
    {
        void (*draw)(lua_State *L, struct microb_window *self);
    } vtable;
} microb_window;

typedef enum microb_window_flags
{
    MICROB_WINDOW_FULLSCREEN = 0x0001,
    MICROB_WINDOW_SHOWN = 0x0002,
    MICROB_WINDOW_HIDDEN = 0x0004,
    MICROB_WINDOW_MINIMIZED = 0x0008,
    MICROB_WINDOW_MAXIMIZED = 0x0010,
    MICROB_WINDOW_SKIP_TASKBAR = 0x0020,
    MICROB_WINDOW_UTILITY = 0x0040,
    MICROB_WINDOW_TOOLTIP = 0x0080,
    MICROB_WINDOW_POPUP_MENU = 0x0100,
} microb_window_flags;

#define MICROB_WINDOW_POSITION_UNDEFINED (-1)
void microb_push_window(lua_State *L,
                        int titleidx,
                        int x, int y, int w, int h,
                        int32_t flags);
void microb_release_window(lua_State *L, int windowidx);
int microbL_window_gc(lua_State *L);

#endif //MICROB_WINDOW_H
