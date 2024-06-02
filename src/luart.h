//
// Created by alinarezrangel on 26/05/24.
//

#ifndef MICROB_LUART_H
#define MICROB_LUART_H

#include <lua.h>

#define MICROB_META_WINDOW "microb window"
#define MICROB_WINDOW_LIST_FIELD "microb windows"

int luaopen_microb(lua_State *L);

#endif //MICROB_LUART_H
