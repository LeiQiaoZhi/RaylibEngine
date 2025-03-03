//
// Created by Qiaozhi Lei on 2/28/25.
//

#ifndef EDITORCONTEXT_H
#define EDITORCONTEXT_H

#include "../luautils/LuaManager.h"
#include "../logger/Logger.h"

struct EditorContext {
    EditorContext(LuaManager& luaManager, Logger& logger) : luaManager(luaManager), logger(logger) {}

    LuaManager& luaManager;
    Logger& logger;
};

#endif //EDITORCONTEXT_H
