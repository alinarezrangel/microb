//
// Created by alinarezrangel on 02/06/24.
//

#ifndef MICROB_TEXT_H
#define MICROB_TEXT_H

#include "component.h"

typedef struct
{
    microb_component header;
    int string_ref;
} microb_text;

microb_text* microb_get_text(microb_component *comp);

#define MICROB_TO_TEXT(comp) ((microb_text *) (comp))

microb_text* microb_to_text(lua_State *L, int idx);
void microb_push_text(lua_State *L, int string_idx, int window_idx);

void microb_release_text(lua_State *L, int idx);
int microbL_text_gc(lua_State *L);

#endif //MICROB_TEXT_H
