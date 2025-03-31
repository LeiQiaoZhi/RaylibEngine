#include "raygui.h"

#define RAYGUI_IMPLEMENTATION
#include <string>
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <list>

#include "../common/input/MouseDragState.h"
#include "nodes/Node.h"
#include "Context.h"
#include "NodePropertiesSubView.h"
#include "PreviewSubView.h"
#include "ShaderCompilationSubView.h"
#include "../common/editor/Editor.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------

    // Load start scene
    const int windowWidth = (1800);
    const int windowHeight = (1000);
    const std::string windowName = (std::string("Shader Editor"));
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
    std::list<Node> nodes;
    nodes.emplace_back(node1);
    nodes.front().name = "Final";
    nodes.front().glsl = "finalColor = vec4(color, alpha);";
    nodes.front().AddInput("color", ShaderType::Vec3);
    nodes.front().AddInput("alpha", ShaderType::Float);

    Camera2D camera = {0};
    camera.target = (Vector2){0, 0};
    camera.offset = (Vector2){windowWidth / 2.0f, windowHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Context context(dragState, camera, nodes, &nodes.front());

    Shader bgShader = LoadShader(
        (std::string(INTERNAL_ASSET_DIR) + "/shaders/default.vert").c_str(),
        (std::string(INTERNAL_ASSET_DIR) + "/shaders/bg.frag").c_str()
    );

    PreviewSubView previewSubView(windowWidth / 5, windowWidth / 5);
    NodePropertiesSubView nodePropertiesSubView(windowWidth / 5, windowHeight - previewSubView.GetSize().y);
    ShaderCompilationSubView shaderCompilationSubView(windowWidth / 5, windowHeight / 2);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------
        context.mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
        dragState.Update();
        // reset immediate states
        context.interactionState = InteractionState::None;
        context.connectionOutput = nullptr;
        context.connectionInput = nullptr;

        // update camera
        if (IsKeyDown(KEY_LEFT_ALT)) {
            camera.zoom += ((float) GetMouseWheelMove() * 0.05f);
            camera.zoom = std::clamp(camera.zoom, 0.2f, 3.0f);
        }
        if (context.mouseDragState.isDragging && IsKeyDown(KEY_LEFT_ALT)) {
            camera.target -= context.mouseDragState.delta;
        }

        // test dragging
        for (auto &node: nodes) {
            node.Update(context);
        }
        for (auto &node: nodes) {
            node.Resolve(context);
        }

        if (context.interactionState == InteractionState::Dragging) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
        } else if (context.interactionState == InteractionState::Connecting) {
            SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        // update subview
        previewSubView.Update(context);

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));


        // background
        SetShaderValue(bgShader, GetShaderLocation(bgShader, "resolution"), (float[2]){windowWidth, windowHeight},
                       SHADER_UNIFORM_VEC2);
        SetShaderValue(bgShader, GetShaderLocation(bgShader, "zoom"), &camera.zoom, SHADER_UNIFORM_FLOAT);
        SetShaderValue(bgShader, GetShaderLocation(bgShader, "cameraPos"), (float[2]){camera.target.x, camera.target.y},
                       SHADER_UNIFORM_VEC2);
        BeginShaderMode(bgShader);
        DrawRectangle(0, 0, windowWidth, windowHeight, WHITE);
        EndShaderMode();

        BeginMode2D(camera);
        for (auto &node: nodes) {
            node.OnDraw(context);
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

        DrawTextEx(Editor::GetFont(), TextFormat("Interaction State: [%i]", context.interactionState), {10, 10},
                   Editor::TextSizeF(), 2, WHITE);
        DrawTextEx(Editor::GetFont(),
                   TextFormat("Connecting I: [%s]",
                              context.connectionInput == nullptr ? "NULL" : context.connectionInput->name.c_str()),
                   {10, 30}, Editor::TextSizeF(), 2, WHITE);
        DrawTextEx(Editor::GetFont(),
                   TextFormat("Connecting O: [%s]",
                              context.connectionOutput == nullptr ? "NULL" : context.connectionOutput->name.c_str()),
                   {10, 50}, Editor::TextSizeF(), 2, WHITE);
        DrawTextEx(Editor::GetFont(), TextFormat("Camera Pos: [%f, %f]", camera.target.x, camera.target.y), {10, 70},
                   Editor::TextSizeF(), 2, WHITE);

        std::string typeInfoLabel = context.showTypeInfo ? "Hide Type Info" : "Show Type Info";
        if (GuiButton({10, 90, Editor::TextWidth(typeInfoLabel.c_str()) + Editor::LargeGap(), Editor::TextSize() * 1.5f},
                      typeInfoLabel.c_str())) {
            context.showTypeInfo = !context.showTypeInfo;
        }

        // subviews
        shaderCompilationSubView.Render({0, windowHeight - shaderCompilationSubView.GetSize().y}, context);
        nodePropertiesSubView.Render({windowWidth - nodePropertiesSubView.GetSize().x, 0}, context);
        previewSubView.Render({
            windowWidth - previewSubView.GetSize().x, windowHeight - previewSubView.GetSize().y
        });

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
