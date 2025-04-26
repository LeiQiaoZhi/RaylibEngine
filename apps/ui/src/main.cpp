#include <string>
#include "raylib.h"
#include "sol/sol.hpp"
#include "ui.h"

using namespace UI;
//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------

    // Lua
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    lua.script_file(std::string(ASSET_DIR) + "/lua/test.lua");

    // lua config
    lua.script_file(std::string(ASSET_DIR) + "/lua/config.lua");
    const int windowWidth = lua["windowWidth"].get_or(800);
    const int windowHeight = lua["windowHeight"].get_or(450);
    const std::string windowName = "UI";
    InitWindow(windowWidth, windowHeight, windowName.c_str());

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    //--------------------------------------------------------------------------------------

    // TODO: construct ui hierarchy tree
    UIElement root = UIElement("Root", windowWidth, windowHeight);
    root.padding = 40;
    root.gap = 20;
    UIElement child1a = UIElement("A", 500, 800);
    child1a.children.reserve(2);
    child1a.children.emplace_back("A1", 300, 200);
    child1a.children.emplace_back("A2", 200, 300);
    child1a.mainAxis = VERTICAL;
    UIElement child1b = UIElement("B", 600, 500);
    child1b.children.reserve(2);
    child1b.children.emplace_back("B1", 300, 200);
    child1b.children.emplace_back("B2", 200, 300);
    root.children.reserve(2);
    root.children.emplace_back(std::move(child1a));
    root.children.emplace_back(std::move(child1b));

    std::cout << "Copy count: " << copyCount << std::endl;
    std::cout << "Move count: " << moveCount << std::endl;

    CalculateLayout(root);
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: calculate layout

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        // TODO: draw ui
        DrawUIDebug(root);

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
