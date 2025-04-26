#include "raygui.h"

#include <string>
#include "raylib.h"
#include <iostream>
#include "GCM/Scene.h"
#include "subviews/ConsoleSubView.h"
#include "subviews/GameSubView.h"
#include "subviews/AssetsSubView.h"
#include "subviews/HierarchySubView.h"
#include "subviews/InspectorSubView.h"
#include "sol/sol.hpp"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------

    // Load start scene
    Scene scene;

    // Lua
    sol::state &lua = scene.runtimeContext.luaManager.lua;
    lua.open_libraries(sol::lib::base);

    lua.script_file(std::string(ASSET_DIR) + "/lua/test.lua");

    // lua config
    lua.script_file(std::string(ASSET_DIR) + "/lua/config.lua");
    const int windowWidth = lua["windowWidth"].get_or(800);
    const int windowHeight = lua["windowHeight"].get_or(450);
    const std::string windowName = lua["windowName"].get_or(std::string("Engine"));
    const std::string style = lua["style"].get_or(std::string("dark"));
    const int uiScale = lua["uiScale"].get_or(1);
    InitWindow(windowWidth, windowHeight, windowName.c_str());

    GameSubView gameSubView(windowWidth / 2, windowHeight);
    const ConsoleSubView consoleSubView(windowWidth / 4, windowHeight / 4);
    HierarchySubView hierarchySubView(windowWidth / 4, windowHeight / 2);
    InspectorSubView inspectorSubView(windowWidth / 4, windowHeight * 3 / 4);
    AssetsSubView assetsSubView(windowWidth / 4, windowHeight / 2);

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // GUI: Initialize gui parameters
    GuiLoadStyle((std::string(PROJECT_DIR) + "/vendor/raygui/styles/" + style + "/style_" + style + ".rgs").c_str());

    GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 6);
    GuiSetStyle(SCROLLBAR, BORDER_WIDTH, 0);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 16 * uiScale);

    hierarchySubView.LoadScene(scene);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (scene.isPlayMode)
            scene.Update();
        else
            scene.EditorUpdate();
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        gameSubView.Render(scene, {static_cast<float>(windowWidth) / 2, 0});
        consoleSubView.Render(scene, {0, static_cast<float>(windowHeight) * .75f});
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
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
