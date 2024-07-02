//
// Created by alinarezrangel on 26/05/24.
//

#include <assert.h>
#include <lauxlib.h>
#include <stdbool.h>
#include "utils.h"
#include "luart.h"

void *microb_alloc(lua_State *L, size_t bytes)
{
    if(bytes == 0)
        return NULL;
    void *ud;
    lua_Alloc alloc = lua_getallocf(L, &ud);
    return (*alloc)(ud, NULL, LUA_TNONE, bytes);
}

void *microb_realloc(lua_State *L, void *ptr, size_t cursize, size_t newsize)
{
    if (ptr == NULL)
    {
        assert(cursize == 0);
        return microb_alloc(L, newsize);
    }
    void *ud;
    lua_Alloc alloc = lua_getallocf(L, &ud);
    return (*alloc)(ud, ptr, cursize, newsize);
}

void microb_free(lua_State *L, void *ptr, size_t cursize)
{
    if(ptr == NULL)
        return;
    void *ud;
    lua_Alloc alloc = lua_getallocf(L, &ud);
    (*alloc)(ud, ptr, cursize, 0);
}

void *microb_to_user_data(lua_State *L, int idx, const char *tname)
{
    int ty = lua_type(L, idx);
    void *ud = lua_touserdata(L, idx);
    if(!ud)
    {
        luaL_error(L, "expected %s but got %s", tname, lua_typename(L, ty));
        return NULL;
    }
    lua_getmetatable(L, idx);
    luaL_getmetatable(L, tname);
    bool eq = lua_rawequal(L, -1, -2);
    lua_pop(L, 2);
    if(eq)
    {
        return ud;
    }
    else
    {
        luaL_error(L, "expected %s but got %s", tname, lua_typename(L, ty));
        return NULL;
    }
}

int microb_force_idx(lua_State *L, int idx)
{
    if(idx < 0)
        return lua_gettop(L) - (idx + 1);
    else
        return idx;
}

void microb_get_ref_table(lua_State *L)
{
    lua_getfield(L, LUA_REGISTRYINDEX, MICROB_REF_TABLE_FIELD);
}

int microb_ref(lua_State *L, int idx)
{
    idx = microb_force_idx(L, idx);
    lua_checkstack(L, 2);
    microb_get_ref_table(L);
    lua_pushnil(L);
    lua_copy(L, idx, lua_gettop(L));
    int ref = luaL_ref(L, -2);
    lua_pop(L, 1);
    return ref;
}

void microb_unref(lua_State *L, int ref)
{
    lua_checkstack(L, 1);
    microb_get_ref_table(L);
    luaL_unref(L, -1, ref);
    lua_pop(L, 1);
}

void microb_get(lua_State *L, int ref)
{
    lua_checkstack(L, 1);
    microb_get_ref_table(L);
    lua_geti(L, -1, ref);
    lua_copy(L, lua_gettop(L), lua_gettop(L) - 1);
    lua_pop(L, 1);
}
