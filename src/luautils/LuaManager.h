//
// Created by Qiaozhi Lei on 2/28/25.
//

#ifndef LUAMANAGER_H
#define LUAMANAGER_H

#include <sol/sol.hpp>

class GameObject;

class LuaManager {
public:
    explicit LuaManager() {
        Register();
    }

    void Register();

    void RegisterAtGameObjectLevel(sol::environment &env, GameObject *gameObject);

public:
    sol::state lua;
};


#endif //LUAMANAGER_H
