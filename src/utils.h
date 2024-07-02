//
// Created by alinarezrangel on 26/05/24.
//

#ifndef MICROB_UTILS_H
#define MICROB_UTILS_H

#include <lua.h>
#include <lauxlib.h>

void* microb_alloc(lua_State *L, size_t bytes);
void* microb_realloc(lua_State *L, void *ptr, size_t cursize, size_t newsize);
void microb_free(lua_State *L, void *ptr, size_t cursize);

void* microb_to_user_data(lua_State *L, int idx, const char *tname);

int microb_force_idx(lua_State *L, int idx);
#define microb_copy(L, from, to) lua_copy((L), microb_force_idx((L), from), microb_force_idx((L), to))

void microb_get_ref_table(lua_State *L); // [-0, +1, -]
int microb_ref(lua_State *L, int idx); // [-0, +0, m]
void microb_unref(lua_State *L, int ref); // [-0, +0, -]
void microb_get(lua_State *L, int ref); // [-0, +1, -]

#endif //MICROB_UTILS_H
