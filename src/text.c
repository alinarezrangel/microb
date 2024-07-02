//
// Created by alinarezrangel on 02/06/24.
//

#include "text.h"
#include "luart.h"
#include "utils.h"
#include <freetype/freetype.h>
#include <SDL_ttf.h>

MICROB_DEFINE_GET_FUNC(microb_get_text, microb_text, MICROB_TEXT)

static void microb_text_draw(lua_State *L, microb_window *win, microb_component *self);

microb_text* microb_to_text(lua_State *L, int idx)
{
    return microb_to_user_data(L, idx, MICROB_META_TEXT);
}

void microb_push_text(lua_State *L, int string_idx, int window_idx)
{
    string_idx = microb_force_idx(L, string_idx);
    window_idx = microb_force_idx(L, window_idx);

    lua_pushnil(L);
    microb_window *win = microb_to_user_data(L, window_idx, MICROB_META_WINDOW);
    microb_get(L, win->component_list_ref);
    microb_text *text = lua_newuserdatauv(L, sizeof(microb_text), 0);
    // [nil, wincomps, text]
    microb_init_component(MICROB_COMPONENT(text), MICROB_TEXT);
    text->header.vtable.draw = &microb_text_draw;
    text->string_ref = microb_ref(L, string_idx);
    luaL_setmetatable(L, MICROB_META_TEXT);

    lua_Integer len = luaL_len(L, -2);
    lua_seti(L, -2, len + 1);
    microb_copy(L, -1, -4);
    lua_pop(L, 3);
}

void microb_release_text(lua_State *L, int idx)
{
    microb_text *text = microb_to_text(L, idx);
    if(!text->header.freed)
    {
        microb_unref(L, text->string_ref);
        microb_release_component(L, idx, MICROB_META_TEXT);
    }
}

int microbL_text_gc(lua_State *L)
{
    microb_release_text(L, -1);
    return 0;
}

static void microb_text_draw(lua_State *L, microb_window *win, microb_component *self)
{
    microb_text *text = MICROB_TO_TEXT(self);
    size_t len = 0;
    microb_get(L, text->string_ref);
    // [str]
    const char *string = lua_tolstring(L, -1, &len);
    TTF_Font *sans = TTF_OpenFont("/usr/share/fonts/truetype/ubuntu/UbuntuMono-B.ttf", 10);
    SDL_Color color = { .r = 0, .g = 0, .b = 0, .a = 0xFF };
    SDL_Surface *surf_text = TTF_RenderText_Solid(sans, string, color);
    SDL_Texture *text_text = SDL_CreateTextureFromSurface(win->renderer, surf_text);
    SDL_Rect target = { .x = 10, .y = 10, .w = 100, .h = 75 };
    SDL_RenderCopy(win->renderer, text_text, NULL, &target);
    SDL_DestroyTexture(text_text);
    SDL_FreeSurface(surf_text);
    TTF_CloseFont(sans);
    lua_pop(L, 1);
}
