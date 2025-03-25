//
// Created by Qiaozhi Lei on 2/28/25.
//

#include "LuaManager.h"
#include <raylib.h>
#include "../GCM/GameObject.h"

struct Vec2Wrapper {
    Vector2 vec;

    Vec2Wrapper() : vec{0.f, 0.f} {
    }

    Vec2Wrapper(float x, float y) : vec{x, y} {
    }
};

struct Vec3Wrapper {
    Vector3 vec;

    Vec3Wrapper() : vec{0.f, 0.f, 0.f} {
    }

    Vec3Wrapper(float x, float y, float z) : vec{x, y, z} {
    }

    explicit Vec3Wrapper(Vector3 vector3) : vec(vector3) {
    }
};


void LuaManager::Register() {
    // Time
    lua.set_function("DeltaTime", [this]() {
        return GetFrameTime();
    });

    // Inputs
    lua.set_function("IsKeyPressed", [this](int key) {
        return IsKeyPressed(key);
    });
    lua.set_function("IsKeyDown", [this](int key) {
        return IsKeyDown(key);
    });
    lua.set_function("IsKeyReleased", [this](int key) {
        return IsKeyReleased(key);
    });
    lua.set_function("IsKeyUp", [this](int key) {
        return IsKeyUp(key);
    });
    lua.set_function("GetMousePosition", [this]() {
        return GetMousePosition();
    });
    lua.set_function("IsMouseButtonPressed", [this](int button) {
        return IsMouseButtonPressed(button);
    });
    lua.set_function("IsMouseButtonDown", [this](int button) {
        return IsMouseButtonDown(button);
    });
    lua.set_function("IsMouseButtonReleased", [this](int button) {
        return IsMouseButtonReleased(button);
    });

    // Vectors


    lua.new_usertype<Vec2Wrapper>("Vector2",
                                  sol::constructors<Vec2Wrapper(), Vec2Wrapper(float, float)>(),
                                  "X", [](const Vec2Wrapper &w) { return w.vec.x; },
                                  "Y", [](const Vec2Wrapper &w) { return w.vec.y; },
                                  "SetX", [](Vec2Wrapper &w, float x) { w.vec.x = x; },
                                  "SetY", [](Vec2Wrapper &w, float y) { w.vec.y = y; }
    );

    lua.new_usertype<Vec3Wrapper>("Vector3",
                                  sol::constructors<Vec3Wrapper(), Vec3Wrapper(float, float, float)>(),
                                  "X", [](const Vec3Wrapper &w) { return w.vec.x; },
                                  "Y", [](const Vec3Wrapper &w) { return w.vec.y; },
                                  "Z", [](const Vec3Wrapper &w) { return w.vec.z; },
                                  "SetX", [](Vec3Wrapper &w, float x) { w.vec.x = x; },
                                  "SetY", [](Vec3Wrapper &w, float y) { w.vec.y = y; },
                                  "SetZ", [](Vec3Wrapper &w, float z) { w.vec.z = z; }
    );
}

void LuaManager::RegisterAtGameObjectLevel(sol::environment &env, GameObject *gameObject) {
    // Transform
    env.set_function("GetWorldPosition", [gameObject]() {
        return Vec3Wrapper(gameObject->GetTransform()->GetWorldPosition());
    });
    env.set_function("GetWorldRight", [gameObject]() {
        return Vec3Wrapper(gameObject->GetTransform()->GetWorldRight());
    });
    env.set_function("GetWorldUp", [gameObject]() {
        return Vec3Wrapper(gameObject->GetTransform()->GetWorldUp());
    });
    env.set_function("GetWorldForward", [gameObject]() {
        return Vec3Wrapper(gameObject->GetTransform()->GetWorldForward());
    });
    env.set_function("GetLocalPosition", [gameObject]() {
        return Vec3Wrapper(gameObject->GetTransform()->GetLocalPosition());
    });
    env.set_function("SetLocalPosition", [gameObject](Vec3Wrapper pos) {
        gameObject->GetTransform()->SetLocalPosition(pos.vec);
    });
}
