#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include <iostream>
#include <string>

#include "raygui.h"
#include "logger/Logger.h"
#include "subviews/ConsoleSubView.h"
#include "subviews/GameSubView.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    Logger logger;

    constexpr int screenWidth = 1600;
    constexpr int screenHeight = 900;
    InitWindow(screenWidth, screenHeight, "Raylib CMake Starter");

    const GameSubView gameSubView(screenWidth / 2, screenHeight);
    const ConsoleSubView consoleSubView(screenWidth / 4, screenHeight / 8);

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
    const auto style = "jungle";
    GuiLoadStyle((std::string(PROJECT_DIR) + "/vendor/raygui/styles/" + style + "/style_" + style + ".rgs").c_str());

    GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 6);
    GuiSetStyle(SCROLLBAR, BORDER_WIDTH, 0);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_ORBITAL);


        //----------------------------------------------------------------------------------


        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        gameSubView.Render(camera, {screenWidth / 2, 0});
        consoleSubView.Render(logger, {0, screenHeight * .75});

        int active;
        // GuiToggleGroup((Rectangle){200, 25, 125, 300}, "Toggle group", &active);

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
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
