#include "raygui.h"

#include <string>

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <list>

#include "input/MouseDragState.h"
#include "nodes/Node.h"
#include "nodes/NodeGroup.h"
#include "Context.h"
#include "NodePropertiesSubView.h"
#include "PreviewSubView.h"
#include "ShaderCompilationSubView.h"
#include "editor/Editor.h"

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
    SetExitKey(0); // Disable exit key (Esc) to avoid accidental exits

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
    nodes.front().position = {200, 0};
    nodes.front().glsl = "finalColor = vec4(color, alpha);";
    nodes.front().hlsl = "finalColor = float4(color, alpha);";
    nodes.front().previewOutputIndex = -1;
    nodes.front().AddInput("color", ShaderType::Vec3);
    nodes.front().AddInput("alpha", ShaderType::Float);
    std::list<NodeGroup> nodeGroups;
    nodeGroups.emplace_back("Selection", true);

    Camera2D camera = {0};
    camera.target = (Vector2){0, 0};
    camera.offset = (Vector2){windowWidth / 2.0f, windowHeight / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Context context(dragState, camera, nodes, nodeGroups);
    context.mainTexture = LoadTexture(INTERNAL_ASSET_DIR "/textures/test.png");
    context.compileFlag = true;

    Shader bgShader = LoadShader(
        (std::string(INTERNAL_ASSET_DIR) + "/shaders/default.vert").c_str(),
        (std::string(INTERNAL_ASSET_DIR) + "/shaders/bg.frag").c_str()
    );

    PreviewSubView previewSubView(windowWidth / 5, windowWidth / 5);
    NodePropertiesSubView nodePropertiesSubView(windowWidth / 5, windowHeight - previewSubView.GetSize().y);
    ShaderCompilationSubView shaderCompilationSubView(windowWidth / 10, windowHeight / 2);
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------
        context.mousePos = GetScreenToWorld2D(GetMousePosition(), camera);
        context.Update();
        // reset immediate states
        context.interactionState = InteractionState::None;
        context.connectionOutput = nullptr;
        context.connectionInput = nullptr;

        // update camera
        if (IsKeyDown(KEY_LEFT_ALT)) {
            camera.zoom += ((float) GetMouseWheelMove() * 0.05f);
            camera.zoom = std::clamp(camera.zoom, 0.5f, 3.0f);
            context.interactionState = InteractionState::Camera;
        }
        if (context.mouseDragState.isDragging && IsKeyDown(KEY_LEFT_ALT)) {
            SetMouseCursor(MOUSE_CURSOR_POINTING_HAND);
            camera.target -= context.mouseDragState.delta;
            context.interactionState = InteractionState::Camera;
        }

        for (auto &group: nodeGroups) {
            group.Update(context);
        }
        for (auto &node: nodes) {
            node.Update(context);
        }

        for (auto &group: nodeGroups) {
            group.Resolve(context);
        }
        for (auto &node: nodes) {
            node.Resolve(context);
        }

        // selection
        if (dragState.JustFinishedDragging() && context.interactionStateLowerThan(InteractionState::Dragging)) {
            context.SelectionGroup()->Clear();
            const Vector2 startMouseWorldPos = GetScreenToWorld2D(dragState.startDragPosition, context.camera);
            Rectangle dragRect = RaylibUtils::GetRectFromPoints(startMouseWorldPos, context.mousePos);
            for (auto &node: nodes) {
                if (CheckCollisionRecs(dragRect, {node.position.x, node.position.y, node.size.x, node.size.y})) {
                    context.SelectionGroup()->AddNode(&node);
                }
            }
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
        for (auto &group: nodeGroups) {
            group.OnDraw(context);
        }
        for (auto &node: nodes) {
            node.OnDraw(context);
        }

        // debug
        const Vector2 mousePos = context.mousePos;
        // DrawText(TextFormat("Mouse Position: [%i, %i]", (int) mousePos.x, (int) mousePos.y), mousePos.x, mousePos.y, 10,
        //          WHITE);
        // TODO: connecting preview
        if (dragState.isDragging) {
            Vector2 mouseStartWorld = GetScreenToWorld2D(dragState.startDragPosition, camera);
            if (context.interactionStateLowerThan(InteractionState::Dragging)) {
                // box selection
                DrawRectangleLinesEx(
                    RaylibUtils::GetRectFromPoints(mouseStartWorld, mousePos),
                    1, YELLOW);
            } else {
                DrawLineV(mouseStartWorld, mousePos, YELLOW);
            }
        }

        EndMode2D();
        Vector2 pos = {10, 10};
        DrawTextEx(Editor::GetFont(),
                   TextFormat("Interaction State: [%s]", InteractionStateToStringMap[context.interactionState].c_str()),
                   pos, Editor::TextSizeF(), 2, WHITE);
        pos.y += Editor::TextSizeF() + Editor::SmallGap();
        DrawTextEx(Editor::GetFont(),
                   TextFormat("Connecting I: [%s]",
                              context.connectionInput == nullptr ? "NULL" : context.connectionInput->name.c_str()),
                   pos, Editor::TextSizeF(), 2, WHITE);
        pos.y += Editor::TextSizeF() + Editor::SmallGap();
        DrawTextEx(Editor::GetFont(),
                   TextFormat("Connecting O: [%s]",
                              context.connectionOutput == nullptr ? "NULL" : context.connectionOutput->name.c_str()),
                   pos, Editor::TextSizeF(), 2, WHITE);
        pos.y += Editor::TextSizeF() + Editor::SmallGap();
        DrawTextEx(Editor::GetFont(), TextFormat("Camera Pos: [%f, %f], Zoom: [%f]", camera.target.x, camera.target.y,
                                                 camera.zoom), pos, Editor::TextSizeF(), 2, WHITE);
        pos.y += Editor::TextSizeF() + Editor::SmallGap();
        // DrawTextEx(Editor::GetFont(), TextFormat("Selected Node: [%d]", context.selectedNodeUID), pos,
        //            Editor::TextSizeF(), 2, WHITE);
        // pos.y += Editor::TextSizeF() + Editor::SmallGap();
        DrawTextEx(Editor::GetFont(),
                   TextFormat("Selected Line: [%s]",
                              context.selectedLine == nullptr ? "NULL" : context.selectedLine->name.c_str()),
                   pos, Editor::TextSizeF(), 2, WHITE);
        pos.y += Editor::TextSizeF() + Editor::SmallGap();

        std::string typeInfoLabel = context.showTypeInfo ? "Hide Type Info" : "Show Type Info";
        if (GuiButton(
            {pos.x, pos.y, Editor::TextWidth(typeInfoLabel.c_str()) + Editor::LargeGap(), Editor::TextSize() * 1.5f},
            typeInfoLabel.c_str())) {
            context.showTypeInfo = !context.showTypeInfo;
        }
        pos.y += Editor::TextSizeF() * 1.5 + Editor::SmallGap();
        // show preview
        GuiComboBox({pos.x, pos.y, 150, Editor::TextSize() * 1.5f},
                    "Individual;Global On;Global Off", reinterpret_cast<int *>(&context.showPreviewState)
        );
        pos.y += Editor::TextSizeF() * 1.5 + Editor::SmallGap();


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
