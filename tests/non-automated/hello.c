#include <lua.h>
#include <lauxlib.h>
#include "../../src/luart.h"
#include "microb/window.h"

static int real_main(lua_State *L)
{
    luaopen_microb(L);
    lua_setglobal(L, "microb");
    luaL_loadstring(L, "win = microb.create_window('hello, world', -1, -1, 300, 300);"
                       "microb.create_text(win, 'hello, world');"
                       " microb.main_loop()");
    lua_call(L, 0, 0);
    return 0;
}

static int msgh(lua_State *L)
{
    const char *msg = lua_tostring(L, 1);
    if (msg == NULL) {  /* is error object not a string? */
        if (luaL_callmeta(L, 1, "__tostring") &&  /* does it have a metamethod */
            lua_type(L, -1) == LUA_TSTRING)  /* that produces a string? */
            return 1;  /* that is the message */
        else
            msg = lua_pushfstring(L, "(error object is a %s value)",
                                  luaL_typename(L, 1));
    }
    luaL_traceback(L, L, msg, 1);  /* append a standard traceback */
    return 1;  /* return the traceback */
}

int main(int argc, char **argv)
{
    lua_State *L = luaL_newstate();
    microb_global_init();
    lua_pushcfunction(L, &msgh);
    lua_pushcfunction(L, &real_main);
    lua_checkstack(L, 100);
    int retcode = 0;
    if(lua_pcall(L, 0, 0, lua_gettop(L) - 1) != LUA_OK)
    {
        const char* str = lua_tostring(L, -1);
        printf("error: %s\n", str);
        retcode = 1;
    }
    lua_close(L);
    microb_global_uninit();
    return retcode;
}
