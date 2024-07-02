//
// Created by alinarezrangel on 25/05/24.
//

#include <lauxlib.h>
#include <SDL_ttf.h>

#include "microb/window.h"
#include "luart.h"
#include "utils.h"
#include "text.h"

void microb_global_init(void)
{
    if(SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        fprintf(stderr, "Could not initialize: %s\n", SDL_GetError());
        abort();
    };
    TTF_Init();
}

void microb_global_uninit(void)
{
    TTF_Quit();
    SDL_Quit();
}

static int32_t microb_translate_window_flags(int32_t f)
{
    int32_t out = 0;
#define MICROB_WINDOW_FLAG_EQ(m, s) \
    if(f & m)\
        out |= s;

    MICROB_WINDOW_FLAG_EQ(MICROB_WINDOW_FULLSCREEN, SDL_WINDOW_FULLSCREEN);
    MICROB_WINDOW_FLAG_EQ(MICROB_WINDOW_HIDDEN, SDL_WINDOW_HIDDEN);
    MICROB_WINDOW_FLAG_EQ(MICROB_WINDOW_SHOWN, SDL_WINDOW_SHOWN);
    MICROB_WINDOW_FLAG_EQ(MICROB_WINDOW_MAXIMIZED, SDL_WINDOW_MAXIMIZED);
    MICROB_WINDOW_FLAG_EQ(MICROB_WINDOW_MINIMIZED, SDL_WINDOW_MINIMIZED);
    MICROB_WINDOW_FLAG_EQ(MICROB_WINDOW_SKIP_TASKBAR, SDL_WINDOW_SKIP_TASKBAR);
    MICROB_WINDOW_FLAG_EQ(MICROB_WINDOW_UTILITY, SDL_WINDOW_UTILITY);
    MICROB_WINDOW_FLAG_EQ(MICROB_WINDOW_TOOLTIP, SDL_WINDOW_TOOLTIP);
    MICROB_WINDOW_FLAG_EQ(MICROB_WINDOW_POPUP_MENU, SDL_WINDOW_POPUP_MENU);
    return out;
}

static void microb_window_draw(lua_State *L, struct microb_window *self);

void microb_push_window(lua_State *L,
                        int titleidx,
                        int x, int y, int w, int h,
                        int32_t flags)
{
    int titleT = lua_type(L, titleidx);
    const char *title = lua_tostring(L, titleidx);
    if(!title)
    {
        luaL_error(L, "expected string for window title, but got %s",
                   lua_typename(L, titleT));
        return;
    }

    lua_newtable(L);
    int component_list_ref = microb_ref(L, -1);
    lua_pop(L, 1);

    microb_window *win = lua_newuserdatauv(L, sizeof(microb_window), 0);
    win->backing_window = SDL_CreateWindow(
            title, x, y, w, h,
            microb_translate_window_flags(flags)
    );
    win->freed = false;
    win->renderer = SDL_CreateRenderer(win->backing_window, -1, SDL_RENDERER_ACCELERATED);
    win->component_list_ref = component_list_ref;
    win->vtable.draw = &microb_window_draw;
    luaL_setmetatable(L, MICROB_META_WINDOW);

    // [win]
    lua_getfield(L, LUA_REGISTRYINDEX, MICROB_WINDOW_LIST_FIELD);
    lua_pushnil(L);
    lua_copy(L, lua_gettop(L) - 2, lua_gettop(L));
    // [win, wlist, win]
    lua_Integer n = luaL_len(L, -2);
    lua_seti(L, -2, n + 1);
    lua_pop(L, 1);
    // [win]
}

static void microb_window_draw(lua_State *L, struct microb_window *self)
{
    SDL_SetRenderDrawColor(self->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(self->renderer);
    SDL_SetRenderDrawColor(self->renderer, 0xFF, 0xFF, 0x00, 0xFF);
    SDL_Rect rect = {.x = 20, .y = 20, .w = 100, .h = 100};
    SDL_RenderFillRect(self->renderer, &rect);

    microb_get(L, self->component_list_ref);
    // [comps]
    lua_Integer n = luaL_len(L, -1);
    for(lua_Integer i = 1; i <= n; i++)
    {
        lua_geti(L, -1, i);
        // [comps, el]
        microb_text *text = microb_to_text(L, -1);
        (*text->header.vtable.draw)(L, self, MICROB_COMPONENT(text));
        lua_pop(L, 1);
    }
    lua_pop(L, 1);
}

void microb_release_window(lua_State *L, int windowidx)
{
    microb_window *win = microb_to_user_data(L, windowidx, MICROB_META_WINDOW);
    if(!win->freed)
    {
        microb_unref(L, win->component_list_ref);

        SDL_DestroyRenderer(win->renderer);
        SDL_DestroyWindow(win->backing_window);
        win->freed = true;
    }
}

int microbL_window_gc(lua_State *L)
{
    microb_release_window(L, -1);
    return 0;
}
