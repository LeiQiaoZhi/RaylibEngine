//
// Created by Qiaozhi Lei on 2/28/25.
//

#ifndef RUNTIMECONTEXT_H
#define RUNTIMECONTEXT_H

#include "../luautils/LuaManager.h"
#include "../logger/Logger.h"

struct RuntimeContext {
    RuntimeContext(LuaManager& luaManager, Logger& logger) : luaManager(luaManager), logger(logger) {}

    LuaManager& luaManager;
    Logger& logger;
};

#endif //RUNTIMECONTEXT_H
