#include "raylib.h"

#define RAYGUI_IMPLEMENTATION
#include <iostream>
#include <string>

#include "raygui.h"
#include "logger/Logger.h"
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
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_FREE);
        //----------------------------------------------------------------------------------

        // Prepare sub views
        gameSubView.Begin(camera);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        gameSubView.Show({screenWidth * .5, 0});

        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        GuiListView((Rectangle){25, 25, 125, 200}, "Text list", NULL, 0);

        int active;
        // GuiToggleGroup((Rectangle){200, 25, 125, 300}, "Toggle group", &active);
        Rectangle panelRec = {20, 400, 200, 150};
        Rectangle panelContentRec = {0, 0, 340, 340};
        Rectangle panelView = {0};
        Vector2 panelScroll = {99, -20};
        GuiScrollPanel(panelRec, "Scroll", panelContentRec, &panelScroll, &panelView);
        // GuiScrollPanel((Rectangle){200, 25, 125, 300}, "Scroll",
        //     (Rectangle){0, 0, 100, 100}, (Vector2) {0, 0},
        // GuiGroupBox((Rectangle){200, 25, 125, 300}, "Group box");

        DrawText(("Panel View:" + std::to_string(panelView.x) + ", " + std::to_string(panelView.y)).c_str(),
                 4, 4, 20, RED);
        DrawText(("Panel Scroll:" + std::to_string(panelScroll.x) + ", " + std::to_string(panelScroll.y)).c_str(),
                 4, 40, 20, RED);

        if (GuiButton((Rectangle){25, 255, 125, 30}, "Push me!")) {
            logger.Log("Button pushed!");
        }

        // Add a checkbox
        GuiCheckBox((Rectangle){25, 290, 20, 20}, "Check me", &checked);

        DrawFPS(10, 10);

        BeginScissorMode(panelView.x, panelView.y, panelView.width, panelView.height);
        auto logEntries = logger.GetLogEntries();
        const Color textColor = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
        for (int i = 0; i < logEntries.size(); i++) {
            DrawText(logEntries[i].message.c_str(),
                     panelView.x + panelScroll.x,
                     panelView.y + panelScroll.y + i * 25,
                     20, textColor);
        }
        EndScissorMode();

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
