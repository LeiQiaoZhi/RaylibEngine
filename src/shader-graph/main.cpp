#include "raygui.h"

#define RAYGUI_IMPLEMENTATION
#include <string>
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

#include "raylib.h"
#include <iostream>

#include "../common/input/MouseDragState.h"
#include "nodes/Node.h"
#include "Context.h"
#include "../common/editor/Editor.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------

    // Load start scene
    const int windowWidth = (1600);
    const int windowHeight = (900);
    const std::string windowName = (std::string("Engine"));
    const std::string style = (std::string("dark"));
    const int uiScale = (1);
    InitWindow(windowWidth, windowHeight, windowName.c_str());

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // GUI: Initialize gui parameters
    GuiLoadStyle((std::string(PROJECT_DIR) + "/vendor/raygui/styles/" + style + "/style_" + style + ".rgs").c_str());

    GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 6);
    GuiSetStyle(SCROLLBAR, BORDER_WIDTH, 0);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 16 * uiScale);

    // test set up
    MouseDragState dragState;
    Node node1;
    Node node2;
    node1.inputs.push_back({"input1", nullptr});
    node1.inputs.push_back({"2", nullptr});
    node1.outputs.push_back({"output1", nullptr});
    node1.outputs.push_back({"R", nullptr});
    node2.inputs.push_back({"A", nullptr});
    node2.inputs.push_back({"input B", nullptr});
    node2.outputs.push_back({"O", nullptr});
    node2.outputs.push_back({"output 2", nullptr});
    std::vector<Node> nodes = {node1, node2};

    Camera2D camera = {0};
    camera.target = (Vector2){0, 0};
    camera.offset = (Vector2){windowWidth / 2.0f, windowHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Context context(dragState, camera);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------
        context.mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
        context.interactionState = InteractionState::None;

        // TODO: update camera

        // TODO: test dragging
        dragState.Update();
        for (auto &node: nodes) {
            node.Update(context);
        }

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        BeginMode2D(camera);
        for (auto &node: nodes) {
            node.OnDraw();
        }

        // debug
        const Vector2 mousePos = context.mousePos;
        DrawText(TextFormat("Mouse Position: [%i, %i]", (int) mousePos.x, (int) mousePos.y), mousePos.x, mousePos.y, 10,
                 WHITE);
        if (dragState.isDragging) {
            Vector2 mouseStart = GetScreenToWorld2D(dragState.startDragPosition, camera);
            DrawLineV(mouseStart, mousePos, YELLOW);
        }

        EndMode2D();

        DrawTextEx(Editor::GetFont(), TextFormat("Interaction State: [%i]", context.interactionState), {10, 10}, Editor::TextSizeF(), 2, WHITE);
        DrawTextEx(Editor::GetFont(), TextFormat("Connecting I: [%s]", context.connectionInput == nullptr ? "NULL" : context.connectionInput->name.c_str()), {10, 30}, Editor::TextSizeF(), 2, WHITE);
        DrawTextEx(Editor::GetFont(), TextFormat("Connecting O: [%s]", context.connectionOutput == nullptr ? "NULL" : context.connectionOutput->name.c_str()), {10, 50}, Editor::TextSizeF(), 2, WHITE);

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
