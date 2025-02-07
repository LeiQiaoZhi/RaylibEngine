#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include <iostream>
#include <string>

#include "raygui.h"
#include "Scene.h"
#include "logger/Logger.h"
#include "subviews/ConsoleSubView.h"
#include "subviews/GameSubView.h"
#include "luautils/LuaUtils.h"
#include "subviews/HierarchySubView.h"

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

    LuaUtils::LuaDoFile(L, std::string(PROJECT_DIR) + "/lua/test.lua");

    // lua config
    LuaUtils::LuaDoFile(L, std::string(PROJECT_DIR) + "/lua/config.lua");
    const int windowWidth = LuaUtils::LuaGetIntegerOrDefault(L, "windowWidth", 800);
    const int windowHeight = LuaUtils::LuaGetIntegerOrDefault(L, "windowHeight", 450);
    const std::string windowName = LuaUtils::LuaGetStringOrDefault(L, "windowName", "Engine");
    const std::string style = LuaUtils::LuaGetStringOrDefault(L, "style", "dark");
    const int cameraMode = LuaUtils::LuaGetIntegerOrDefault(L, "cameraMode", CAMERA_ORBITAL);

    InitWindow(windowWidth, windowHeight, windowName.c_str());

    const GameSubView gameSubView(windowWidth / 2, windowHeight);
    const ConsoleSubView consoleSubView(windowWidth / 4, windowHeight / 4);
    const HierarchySubView hierarchySubView(windowWidth / 4, windowHeight / 2);

    // Variables
    bool showTextInputBox = false;
    bool checked = false;

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
    scene.root = new GameObject("Root Node", 1);
    GameObject cameraGO("Camera", 2);
    scene.root->AddChild(&cameraGO);
    GameObject testParent("Test Parent", 3);
    scene.root->AddChild(&testParent);
    GameObject testChild1("Test Child 1", 4);
    testParent.AddChild(&testChild1);
    GameObject testChild2("Test Child 2", 5);
    testParent.AddChild(&testChild2);

    //--------------------------------------------------------------------------------------

    int active;
    int current = 0;
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, cameraMode);
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

        gameSubView.Render(camera, {static_cast<float>(windowWidth) / 2, 0});
        consoleSubView.Render(logger, {0, static_cast<float>(windowHeight) * .75f});
        hierarchySubView.Render(scene, {static_cast<float>(windowWidth) * .25f, static_cast<float>(windowHeight) * .5f});

        GuiListView((Rectangle){10, 25, 50, 300}, "One;Two;Three;Four;Five", nullptr, &active);
        GuiToggleGroup((Rectangle){200, 25, 50, 50}, "One;Two;Three;Four;Five", &active);
        Vector2 mouseCell;
        GuiGrid((Rectangle){200, 100, 100, 100}, "One;Two;Three;Four;Five", current * 10, 1, &mouseCell);
        GuiSpinner((Rectangle){200, 200, 100, 30}, "Spinner", &current, 0, 100, true);
        GuiTooltip(Rectangle{200, 250, 100, 300});
        const char *items[] = {"A", "B", "C"};
        auto toClose = GuiTabBar((Rectangle){200, 250, 100, 30}, items, 3, &current);
        if (toClose != -1)
            logger.Log(TextFormat("Tab bar closed: %d", toClose));

        GuiComboBox((Rectangle){200, 300, 100, 30}, "A;B;C", &current);

        // GuiGroupBox((Rectangle){200, 25, 125, 300}, "Group box");

        // Add a checkbox
        GuiCheckBox((Rectangle){25, 290, 20, 20}, "Check me", &checked);

        DrawFPS(10, 10);

        if (GuiButton((Rectangle){25, 255, 125, 30}, "Push me!")) {
            logger.Log(TextFormat("Button pushed! %d", GetRandomValue(0, 100)));
        }

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
