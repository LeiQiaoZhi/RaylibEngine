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

    // construct ui hierarchy tree
    UIElement root = UIBuilder{}.setName("Root")
            .setSize(windowWidth, windowHeight)
            .setPadding(40).setGap(20)
            .setChildren({
                UIBuilder{}.setName("A")
                .setSize(FIT, GROW).setMainAxis(VERTICAL)
                .setChildren({
                    UIBuilder{}.setName("A1").setSize(300, GROW),
                    UIBuilder{}.setName("A2").setSize(400, GROW),
                    UIBuilder{}.setName("A3").setSize(300, GROW)
                }),
                UIBuilder{}.setName("B")
                .setSize(GROW, GROW).setPadding(50)
                .setChildren({
                    UIBuilder{}.setName("B1").setSize(300, 400),
                    UIBuilder{}.setName("B2").setSize(400, 300)
                }),
                UIBuilder{}.setName("C")
                .setSize(GROW, GROW)
            });

    std::cout << "Copy count: " << copyCount << std::endl;
    std::cout << "Move count: " << moveCount << std::endl;

    CalculateLayout(root);
    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: calculate layout
        root.width = GetScreenWidth();
        root.height = GetScreenHeight();
        if (IsKeyPressed(KEY_SPACE))
           CalculateLayout(root);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        // TODO: draw ui
        DrawUIDebug(root);

        DrawFPS(GetScreenWidth() - 80, 0);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
