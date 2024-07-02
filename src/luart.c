//
// Created by alinarezrangel on 26/05/24.
//

#include <lauxlib.h>
#include "luart.h"
#include "utils.h"
#include "microb/window.h"
#include "text.h"

static int microb_lua_int_to_int(lua_Integer i)
{
    if(i > INT_MAX)
        return INT_MAX;
    else if(i < INT_MIN)
        return INT_MIN;
    else
        return (int) i;
}

static int microbL_create_window(lua_State *L)
{
    luaL_checkstring(L, 1);
    int x, y, w, h;
    if(lua_isnil(L, 1))
        x = MICROB_WINDOW_POSITION_UNDEFINED;
    else
        x = microb_lua_int_to_int(luaL_checkinteger(L, 2));
    if(lua_isnil(L, 2))
        y = MICROB_WINDOW_POSITION_UNDEFINED;
    else
        y = microb_lua_int_to_int(luaL_checkinteger(L, 3));
    w = microb_lua_int_to_int(luaL_checkinteger(L, 4));
    h = microb_lua_int_to_int(luaL_checkinteger(L, 5));
    microb_push_window(L, 1, x, y, w, h, MICROB_WINDOW_SHOWN);
    return 1;
}

static int microbL_create_text(lua_State *L)
{
    luaL_checkudata(L, 1, MICROB_META_WINDOW);
    luaL_checkstring(L, 2);
    microb_push_text(L, 2, 1);
    return 1;
}

static int microbL_main_loop(lua_State *L)
{
    lua_getfield(L, LUA_REGISTRYINDEX, MICROB_WINDOW_LIST_FIELD);
    lua_len(L, -1);
    // [wlist, l]
    lua_Integer n = lua_tointeger(L, -1);
    lua_pop(L, 1);
    // [wlist]

    while(true)
    {
        SDL_Event ev;
        if(SDL_WaitEvent(&ev) == 0)
        {
            return luaL_error(L,
                              "Error while waiting for an event: %s\n",
                              SDL_GetError());
        }

        if(ev.type == SDL_QUIT)
            break;

        for(lua_Integer i = 1; i <= n; i++)
        {
            lua_geti(L, -1, i);
            microb_window *win = microb_to_user_data(
                    L,
                    -1,
                    MICROB_META_WINDOW
            );
            if(win->freed)
            {
                lua_pop(L, 1);
                continue;
            }
            (*win->vtable.draw)(L, win);
            SDL_RenderPresent(win->renderer);
            lua_pop(L, 1);
        }
    }
    return 0;
}

static luaL_Reg regs[] = {
        {"create_window", microbL_create_window },
        {"create_text", microbL_create_text},
        {"main_loop", microbL_main_loop},
};

int luaopen_microb(lua_State *L)
{
    luaL_newmetatable(L, MICROB_META_WINDOW);
    lua_pushcfunction(L, &microbL_window_gc);
    lua_setfield(L, -2, "__gc");
    lua_pop(L, 1);

    luaL_newmetatable(L, MICROB_META_TEXT);
    lua_pushcfunction(L, &microbL_text_gc);
    lua_setfield(L, -2, "__gc");
    lua_pop(L, 1);

    lua_newtable(L);
    lua_setfield(L, LUA_REGISTRYINDEX, MICROB_WINDOW_LIST_FIELD);

    lua_newtable(L);
    lua_setfield(L, LUA_REGISTRYINDEX, MICROB_REF_TABLE_FIELD);

    luaL_newlib(L, regs);
    return 1;
}

void microb_print_stack(lua_State *L)
{
    printf("%d elements\n", lua_gettop(L));
    int top = lua_gettop(L);
    lua_pushnil(L);
    for(int i = 1; i <= top; i++)
    {
        lua_copy(L, i, top + 1);
        const char *value = lua_tostring(L, top + 1);
        printf("  %d: %s %s\n", i, lua_typename(L, lua_type(L, i)), value);
    }
    lua_pop(L, 1);
}
