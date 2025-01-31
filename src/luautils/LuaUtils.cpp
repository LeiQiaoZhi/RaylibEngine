#include "LuaUtils.h"
#include <iostream>

int LuaUtils::LuaDoFile(lua_State *L, const char *filename) {
    const int status = luaL_dofile(L, filename);
    if (status != LUA_OK) {
        std::cerr << "Error loading file: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }
    return status;
}

int LuaUtils::LuaDoFile(lua_State *L, const std::string &filename) {
    return LuaDoFile(L, filename.c_str());
}

int LuaUtils::LuaCallVoidFunction(lua_State *L, const char *functionName) {
    lua_getglobal(L, functionName);
    if (!lua_isfunction(L, -1)) {
        std::cerr << functionName << " is not a function" << std::endl;
        lua_pop(L, 1);
        return LUA_ERRRUN;
    }
    const int status = lua_pcall(L, 0, 0, 0);
    if (status != LUA_OK) {
        std::cerr << "Error calling function: " << functionName << std::endl;
        lua_pop(L, 1);
    }
    return status;
}

int LuaUtils::LuaGetIntegerOrDefault(lua_State *L, const char *name, const int defaultValue = 0) {
    int result = defaultValue;
    lua_getglobal(L, name);
    if (lua_isinteger(L, -1)) {
        result = lua_tointeger(L, -1);
    } else {
        std::cerr << name << " is not an integer" << std::endl;
    }
    lua_pop(L, 1);
    return result;
}

int LuaUtils::LuaGetIntegerOrDefault(lua_State *L, const std::string &name, const int defaultValue = 0) {
    return LuaGetIntegerOrDefault(L, name.c_str(), defaultValue);
}

std::string LuaUtils::LuaGetStringOrDefault(lua_State *L, const char *name, const std::string &defaultValue = "") {
    std::string result = defaultValue;
    lua_getglobal(L, name);
    if (lua_isstring(L, -1)) {
        result = lua_tostring(L, -1);
    } else {
        std::cerr << name << "is not an integer" << std::endl;
    }
    lua_pop(L, 1);
    return result;
}

std::string LuaUtils::LuaGetStringOrDefault(lua_State *L, const std::string &name, const std::string &defaultValue) {
    return LuaGetStringOrDefault(L, name.c_str(), defaultValue);
}
