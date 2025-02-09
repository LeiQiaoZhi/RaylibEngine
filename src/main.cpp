#include "raygui.h"

#define RAYGUI_IMPLEMENTATION
#include <string>
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

#include "raylib.h"
#include <iostream>
#include "GCM/Scene.h"
#include "GCM/Components/ModelComponent.h"
#include "logger/Logger.h"
#include "subviews/ConsoleSubView.h"
#include "subviews/GameSubView.h"
#include "luautils/LuaUtils.h"
#include "subviews/AssetsSubView.h"
#include "subviews/HierarchySubView.h"
#include "subviews/InspectorSubView.h"

Logger logger;

int LuaPrint(lua_State *L) {
    const char *message = lua_tostring(L, 1); // get argument
    if (!message) message = "(error: message is not a string)";

    lua_Debug d;
    if (lua_getstack(L, 1, &d)) {
        lua_getinfo(L, "nSl", &d); // name, source, line
        logger.Log(TextFormat("%s:%d: %s", d.short_src, d.currentline, message));
    } else {
        logger.Log(message);
    }
    return 0;
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------

    // Lua
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "log", LuaPrint);

    LuaUtils::LuaDoFile(L, std::string(ASSET_DIR) + "/lua/test.lua");

    // lua config
    LuaUtils::LuaDoFile(L, std::string(ASSET_DIR) + "/lua/config.lua");
    const int windowWidth = LuaUtils::LuaGetIntegerOrDefault(L, "windowWidth", 800);
    const int windowHeight = LuaUtils::LuaGetIntegerOrDefault(L, "windowHeight", 450);
    const std::string windowName = LuaUtils::LuaGetStringOrDefault(L, "windowName", "Engine");
    const std::string style = LuaUtils::LuaGetStringOrDefault(L, "style", "dark");
    const int cameraMode = LuaUtils::LuaGetIntegerOrDefault(L, "cameraMode", CAMERA_ORBITAL);

    InitWindow(windowWidth, windowHeight, windowName.c_str());

    const GameSubView gameSubView(windowWidth / 2, windowHeight);
    const ConsoleSubView consoleSubView(windowWidth / 4, windowHeight / 4);
    const HierarchySubView hierarchySubView(windowWidth / 4, windowHeight / 2);
    const InspectorSubView inspectorSubView(windowWidth / 4, windowHeight * 3 / 4);
    AssetsSubView assetsSubView(windowWidth / 4, windowHeight / 2);

    // Camera
    Camera camera = {0};
    camera.position = (Vector3){100.0f, 100.0f, 100.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // GUI: Initialize gui parameters
    GuiLoadStyle((std::string(PROJECT_DIR) + "/vendor/raygui/styles/" + style + "/style_" + style + ".rgs").c_str());

    GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 6);
    GuiSetStyle(SCROLLBAR, BORDER_WIDTH, 0);

    // test scene set up
    Scene scene;
    scene.name = "Test Scene";
    scene.root = new GameObject(scene.name, 1);
    GameObject cameraGO("Camera", 2);
    cameraGO.AddComponent(new CameraComponent(&camera, cameraMode));
    scene.root->AddChild(&cameraGO);
    GameObject testParentA("Test Parent A", 3);
    scene.root->AddChild(&testParentA);
    GameObject testChild1("Test Child 1", 4);
    testParentA.AddChild(&testChild1);
    GameObject testChild2("Test Child 2", 5);
    testParentA.AddChild(&testChild2);
    GameObject testParentB("Test Parent B", 6);
    scene.root->AddChild(&testParentB);
    testParentB.AddComponent(new ModelComponent());
    GameObject testChild3("Test Child 3", 7);
    testParentB.AddChild(&testChild3);
    GameObject testChild4("Test Child 4", 8);
    testParentB.AddChild(&testChild4);


    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        scene.UpdateComponents();
        //----------------------------------------------------------------------------------

        // Lua
        lua_getglobal(L, "update");
        if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
            std::cerr << "Error calling function: " << lua_tostring(L, -1) << std::endl;
            lua_pop(L, 1);
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));


        gameSubView.Render(scene, {static_cast<float>(windowWidth) / 2, 0});
        consoleSubView.Render(logger, {0, static_cast<float>(windowHeight) * .75f});
        hierarchySubView.Render(scene, {
                                    static_cast<float>(windowWidth) * .25f, static_cast<float>(windowHeight) * .5f
                                });
        inspectorSubView.Render(scene, {0, 0});
        assetsSubView.Render(scene, {static_cast<float>(windowWidth) * .25f, 0});

        DrawFPS(windowWidth - 80, 0);


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    lua_close(L);
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
