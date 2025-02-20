#ifndef LUAUTILS_H
#define LUAUTILS_H

#include <string>

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}


namespace LuaUtils {
    int LuaDoFile(lua_State *L, const char *filename);

    int LuaDoFile(lua_State *L, const std::string &filename);

    // Functions
    int LuaCallVoidFunction(lua_State *L, const char *functionName);

    int LuaCallVoidFunction(lua_State *L, const std::string &functionName);

    // Global Variables
    int LuaGetIntegerOrDefault(lua_State *L, const char *name, int defaultValue);

    int LuaGetIntegerOrDefault(lua_State *L, const std::string &name, int defaultValue);

    std::string LuaGetStringOrDefault(lua_State *L, const char *name, const std::string &defaultValue);

    std::string LuaGetStringOrDefault(lua_State *L, const std::string &name, const std::string &defaultValue);
}


#endif //LUAUTILS_H
