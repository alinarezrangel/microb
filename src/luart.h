//
// Created by alinarezrangel on 26/05/24.
//

#ifndef MICROB_LUART_H
#define MICROB_LUART_H

#include <lua.h>

#define MICROB_META_WINDOW "microb window"
#define MICROB_META_TEXT "microb text component"
#define MICROB_WINDOW_LIST_FIELD "microb windows"
#define MICROB_REF_TABLE_FIELD "microb references"

int luaopen_microb(lua_State *L);
void microb_print_stack(lua_State *L);

#endif //MICROB_LUART_H
