//
// Created by alinarezrangel on 26/05/24.
//

#ifndef MICROB_UTILS_H
#define MICROB_UTILS_H

#include <lua.h>

void* microb_alloc(lua_State *L, size_t bytes);
void* microb_realloc(lua_State *L, void *ptr, size_t cursize, size_t newsize);
void microb_free(lua_State *L, void *ptr, size_t cursize);

void* microb_to_user_data(lua_State *L, int idx, const char *tname);

#endif //MICROB_UTILS_H
