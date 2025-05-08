#include <string>
#include "raylib.h"
#include "sol/sol.hpp"

#define LAYOUT_IMPLEMENTATION
// #define LAYOUT_VERBOSE
#include "layout.h"
#undef LAYOUT_IMPLEMENTATION
#include "ui.h"
#include <editor/Editor.h>
#include "difficultyPage.h"

void UI_Raylib_Init();

using namespace Layout;

inline void DrawUIDebug(LayoutElement &root) {
    std::vector<LayoutElement *> toExplore = {&root};
    while (!toExplore.empty()) {
        LayoutElement *current = toExplore.back();
        toExplore.pop_back();

        DrawRectangleLines(current->x, current->y, current->width, current->height, YELLOW);

        if (current->width > 200) {
            DrawTextEx(Editor::GetFont(),
                       TextFormat("[%s], %s, %s%ix%s%i, (%i,%i), P%i G%i", current->debugName.c_str(),
                                  current->mainAxis == HORIZONTAL ? "H" : "V",
                                  current->SizingChar(current->widthSizing).c_str(), current->width,
                                  current->SizingChar(current->heightSizing).c_str(), current->height,
                                  current->x, current->y, current->padding, current->gap),
                       {static_cast<float>(current->x), static_cast<float>(current->y)},
                       12, 1, WHITE);
        }
        if (current->width == 0 || current->height == 0) {
            DrawTextEx(Editor::GetFont(),
                       TextFormat("[%s]", current->debugName.c_str()),
                       {static_cast<float>(current->x), static_cast<float>(current->y)},
                       12, 1, RED);
        }

        for (auto &child: current->children) {
            toExplore.emplace_back(&child);
        }
    }
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------

    UI_Raylib_Init();

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

    // SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    //--------------------------------------------------------------------------------------

    // construct ui hierarchy tree
    const LayoutBuilder component = LayoutBuilder{}.name("Component").size(400, GROW)
            .drawFn([](LayoutElement *layout) {
                DrawRectangle(layout->x, layout->y, layout->width, layout->height, WHITE);
            })
            .children({
                LayoutBuilder{}.name("1").size(100, GROW)
                .drawFn([](LayoutElement *layout) {
                    DrawRectangle(layout->x, layout->y, layout->width, layout->height, GRAY);
                    UI::UI_DrawText("1", layout->x, layout->y, 1.0f);
                }),
                LayoutBuilder{}.name("2").size(GROW, GROW)
                .drawFn([](LayoutElement *layout) {
                    DrawRectangle(layout->x, layout->y, layout->width, layout->height, BLACK);
                }),
                LayoutBuilder{}.name("3").size(100, GROW)
                .drawFn([](LayoutElement *layout) {
                    DrawRectangle(layout->x, layout->y, layout->width, layout->height, GRAY);
                }),
            });
    UI::UI_Text text = {
        .text =
        "Because a raw function pointer void(*)(LayoutElement*) can only point to a free function or a non-capturing lambda whose signature exactly matches. Any lambda that closes over (captures) outside state is a unique closure type—it isn’t just a function pointer and can’t convert to DrawFn.",
    };
    LayoutElement root = LayoutBuilder{}.name("Root")
            .size(windowWidth, windowHeight).padding(40).gap(20)
            .drawFn([](LayoutElement *layout) {
                DrawRectangle(layout->x, layout->y, layout->width, layout->height, GRAY);
            })
            .children({
                LayoutBuilder{}.name("A")
                .size(FIT, GROW).mainAxis(VERTICAL)
                .alignment(CENTER, END)
                .drawFn([](LayoutElement *layout) {
                    DrawRectangle(layout->x, layout->y, layout->width, layout->height, BLACK);
                })
                .children({
                    component.copy().name("A1"),
                    component.copy().name("A2"),
                    component.copy().name("A3")
                }),
                // LayoutBuilder{}.name("B")
                // .size(GROW, GROW).padding(50)
                // .children({
                //     LayoutBuilder{}.name("B1").size(300, 400),
                //     LayoutBuilder{}.name("B2").size(400, 300)
                // }),
                LayoutBuilder{}.name("C")
                .size(GROW, GROW)
                .alignment(CENTER, CENTER).mainAxis(VERTICAL)
                .drawFn([](LayoutElement *layout) {
                    DrawRectangle(layout->x, layout->y, layout->width, layout->height, GREEN);
                })
                .children({
                    LayoutBuilder{}.name("C1").size(GROW, 400)
                    .pointer(&text.layout),
                    LayoutBuilder{}.name("C2").size(GROW, GROW)
                    .drawFn([](LayoutElement *layout) {
                        DrawRectangle(layout->x, layout->y, layout->width, layout->height, BLACK);
                    })
                }),
            });
    InitReferencePointers(root);
    text.Link();

    // std::cout << "Copy count: " << copyCount << std::endl;
    // std::cout << "Move count: " << moveCount << std::endl;

    DifficultyPage page;
    bool drawDebug = false;
    bool drawNormal = true;

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        DetectInputEvents(page.root);

        //  calculate layout
        page.root.width = GetScreenWidth();
        page.root.height = GetScreenHeight();
#ifdef LAYOUT_VERBOSE
        if (IsKeyPressed(KEY_SPACE))
#endif
        CalculateLayout(page.root);

        if (IsKeyPressed(KEY_D))
            drawDebug = !drawDebug;
        if (IsKeyPressed(KEY_F))
            drawNormal = !drawNormal;

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        // draw ui
        if (drawNormal)
            DrawUI(page.root);
        if (drawDebug)
            DrawUIDebug(page.root);

        // DrawFPS(GetScreenWidth() - 80, 0);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
