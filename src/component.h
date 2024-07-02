//
// Created by alinarezrangel on 02/06/24.
//

#ifndef MICROB_COMPONENT_H
#define MICROB_COMPONENT_H

#include "microb/window.h"

struct microb_component_vtable;
typedef struct microb_component_vtable microb_component_vtable;
struct microb_component;
typedef struct microb_component microb_component;

typedef enum microb_component_type
{
    MICROB_TEXT,
} microb_component_type;

struct microb_component_vtable
{
    void (*release)(lua_State *L, microb_component *self);

    void (*draw)(lua_State *L, microb_window *win, microb_component *self);
};

struct microb_component
{
    microb_component_type type;
    microb_component_vtable vtable;
    bool freed;
};

#define MICROB_COMPONENT(el) ((microb_component *) (el))

#define MICROB_DEFINE_GET_FUNC(name, stype, ntype) \
stype* microb_get_text(microb_component *comp)     \
{                                                  \
    if(comp->type == ntype)                        \
        return (stype *) comp;                     \
    else                                           \
        return NULL;                               \
}

void microb_init_component(microb_component *self, microb_component_type ty);

void microb_release_component(lua_State *L, int idx, const char *meta);

#endif //MICROB_COMPONENT_H
