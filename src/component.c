//
// Created by alinarezrangel on 02/06/24.
//

#include "component.h"
#include "utils.h"

void microb_init_component(microb_component *self, microb_component_type ty)
{
    self->type = ty;
    self->freed = false;
    self->vtable.release = NULL;
    self->vtable.draw = NULL;
}

void microb_release_component(lua_State *L, int idx, const char *meta)
{
    microb_component *comp = microb_to_user_data(L, idx, meta);
    if(!comp->freed)
    {
        if(comp->vtable.release)
            (*comp->vtable.release)(L, comp);
        comp->freed = true;
    }
}
