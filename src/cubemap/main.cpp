#include "raygui.h"

#define RAYGUI_IMPLEMENTATION
#include <string>
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

#include "raylib.h"
#include "../common/editor/Editor.h"
#include "sol/sol.hpp"
#include "CubemapUtils.h"

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
    const std::string windowName = "Cubemap";
    const std::string style = lua["style"].get_or(std::string("dark"));
    const int uiScale = lua["uiScale"].get_or(1);
    InitWindow(windowWidth, windowHeight, windowName.c_str());

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    // GUI: Initialize gui parameters
    GuiLoadStyle((std::string(PROJECT_DIR) + "/vendor/raygui/styles/" + style + "/style_" + style + ".rgs").c_str());

    GuiSetStyle(LISTVIEW, SCROLLBAR_WIDTH, 6);
    GuiSetStyle(SCROLLBAR, BORDER_WIDTH, 0);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 16 * uiScale);

    //--------------------------------------------------------------------------------------
    Texture2D hdrTexture = Texture();
    TextureCubemap cubemapTexture = TextureCubemap();
    TextureCubemap irradianceTexture;
    Shader hdrToCubemapShader = LoadShader(
        INTERNAL_ASSET_DIR "/shaders/default.vert",
        INTERNAL_ASSET_DIR "/shaders/cubemap.frag"
    );
    SetShaderValue(hdrToCubemapShader, GetShaderLocation(hdrToCubemapShader, "equirectangularMap"), (int[1]){0},
                   SHADER_UNIFORM_INT);


    Camera camera;
    camera.fovy = 45.0;
    camera.position = {0.0f, 1.0f, 1.0f};
    camera.target = {0.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.projection = CAMERA_PERSPECTIVE;

    Model skybox;
    Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    skybox = LoadModelFromMesh(cube);
    Material &material = skybox.materials[skybox.meshMaterial[0]];
    skybox.materials[skybox.meshMaterial[0]].shader = LoadShader(
        ASSET_DIR "/shaders/custom.vert",
        ASSET_DIR "/shaders/skybox.frag"
    );
    skybox.materials[skybox.meshMaterial[0]].shader.locs[SHADER_LOC_MAP_CUBEMAP] = GetShaderLocation(
        material.shader, "environmentMap");


    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera, CAMERA_FREE);

        if (IsFileDropped()) {
            FilePathList droppedFiles = LoadDroppedFiles();
            std::cout << "Dropped files count: " << droppedFiles.count << std::endl;

            if (droppedFiles.count == 1 && IsFileExtension(droppedFiles.paths[0], ".png")) {
                std::cout << "Dropped png file: " << droppedFiles.paths[0] << std::endl;
                UnloadTexture(cubemapTexture);
                Image cubemapImage = LoadImage(droppedFiles.paths[0]);
                cubemapTexture = LoadTextureCubemap(cubemapImage, CUBEMAP_LAYOUT_AUTO_DETECT);
                skybox.materials[skybox.meshMaterial[0]].maps[MATERIAL_MAP_CUBEMAP].texture = cubemapTexture;
                UnloadImage(cubemapImage);
            }
            if (droppedFiles.count == 1 && IsFileExtension(droppedFiles.paths[0], ".hdr")) {
                std::cout << "Dropped hdr file: " << droppedFiles.paths[0] << std::endl;
                UnloadTexture(hdrTexture);
                hdrTexture = LoadTexture(droppedFiles.paths[0]);
            }

            UnloadDroppedFiles(droppedFiles); // Unload filepaths from memory
        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));


        if (IsTextureValid(cubemapTexture)) {
            BeginMode3D(camera);
            DrawGrid(10, 1.0f);
            // Draw skybox
            rlDisableDepthMask();
            rlDisableBackfaceCulling();
            Material *material = &skybox.materials[skybox.meshMaterial[0]];
            // SetShaderValue(material->shader, GetShaderLocation(material->shader, "flipY"), &skyboxFlipY,
            //                SHADER_UNIFORM_INT);
            SetShaderValue(material->shader, GetShaderLocation(material->shader, "viewPos"),
                           &camera.position, SHADER_UNIFORM_VEC3);
            DrawModel(skybox, camera.position, 1.0f, WHITE);
            rlEnableBackfaceCulling();
            rlEnableDepthMask();
            EndMode3D();
        }


        DrawFPS(windowWidth - 80, 0);


        Rectangle rect = {Editor::MediumGap(), Editor::MediumGap(), 500, Editor::TextSizeF() * 1.5f};
        GuiLabel(rect, TextFormat("HDR: %s, Cubemap: %s, Irradiance: %s",
                                  IsTextureValid(hdrTexture) ? "Loaded" : "Not loaded",
                                  IsTextureValid(cubemapTexture) ? "Loaded" : "Not loaded",
                                  IsTextureValid(irradianceTexture) ? "Loaded" : "Not loaded"));
        rect.y += rect.height + Editor::SmallGap();
        if (GuiButton(rect, "HDR to cubemap")) {
            if (!IsTextureValid(hdrTexture)) {
                std::cout << "No HDR texture loaded" << std::endl;
            } else {
                std::cout << "HDR to cubemap" << std::endl;
                cubemapTexture = GenTextureCubemap(hdrToCubemapShader, hdrTexture, 1024,
                                                   PIXELFORMAT_UNCOMPRESSED_R8G8B8);
                skybox.materials[skybox.meshMaterial[0]].maps[MATERIAL_MAP_CUBEMAP].texture = cubemapTexture;

                // export
                // Image image = LoadImageFromTexture(cubemapTexture);
                // ExportImage(image, INTERNAL_ASSET_DIR "/textures/cubemap.png");
            }
        }
        rect.y += rect.height + Editor::SmallGap();
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
