#include "raygui.h"

#define RAYGUI_IMPLEMENTATION
#include <string>
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

#include "raylib.h"
#include "../common/editor/Editor.h"
#include "sol/sol.hpp"
#include "CubemapUtils.h"
#include "../common/utils/Utils.h"

enum class PreviewTarget {
    HDR = 0,
    CUBEMAP,
    IRRADIANCE
};

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

    Camera camera;
    camera.fovy = 90.0;
    camera.position = {0.0f, 0.0f, 0.0f};
    camera.target = {1.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.projection = CAMERA_PERSPECTIVE;

    Model skybox;
    Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    skybox = LoadModelFromMesh(cube);
    Material &skyboxMat = skybox.materials[skybox.meshMaterial[0]];
    skyboxMat.shader = LoadShader(
        ASSET_DIR "/shaders/custom.vert",
        ASSET_DIR "/shaders/skybox.frag"
    );
    skyboxMat.shader.locs[SHADER_LOC_MAP_CUBEMAP] = GetShaderLocation(skyboxMat.shader, "environmentMap");

    Model hdrSkybox;
    Mesh cube2 = GenMeshCube(1.0f, 1.0f, 1.0f);
    hdrSkybox = LoadModelFromMesh(cube2);
    Material &hdrMat = hdrSkybox.materials[hdrSkybox.meshMaterial[0]];
    hdrMat.shader = hdrToCubemapShader;
    hdrMat.shader.locs[SHADER_LOC_MAP_DIFFUSE] = GetShaderLocation(hdrMat.shader, "equirectangularMap");

    RenderTexture renderTexture = LoadRenderTexture(512, 512);

    bool skyboxFlipY = false;
    bool updateCamera = false;
    float settingsHeight = 0.0f;
    int targetFace = 0;
    PreviewTarget previewTarget = PreviewTarget::HDR;
    std::string statusText;
    bool statusWarning = false;
    int dimensionIndex = 2;

    // default file
    LoadHDRMap(INTERNAL_ASSET_DIR "/textures/default.hdr", hdrTexture, hdrMat);

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            updateCamera = !updateCamera;
        }
        if (updateCamera) {
            SetMouseCursor(MOUSE_CURSOR_CROSSHAIR);
            UpdateCamera(&camera, CAMERA_FREE);
        } else {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        }

        if (IsFileDropped()) {
            FilePathList droppedFiles = LoadDroppedFiles();
            if (droppedFiles.count == 1 && IsFileExtension(droppedFiles.paths[0], ".png"))
                LoadCubemap(droppedFiles.paths[0], cubemapTexture, skyboxMat);
            if (droppedFiles.count == 1 && IsFileExtension(droppedFiles.paths[0], ".hdr"))
                LoadHDRMap(droppedFiles.paths[0], hdrTexture, hdrMat);
            UnloadDroppedFiles(droppedFiles); // Unload filepaths from memory
        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        if (IsTextureValid(hdrTexture) && previewTarget == PreviewTarget::HDR) {
            BeginMode3D(camera);
            rlDisableDepthMask();
            rlDisableBackfaceCulling();
            int value = skyboxFlipY ? 1 : 0;
            SetShaderValue(hdrMat.shader, GetShaderLocation(hdrMat.shader, "flipY"),
                           &value, SHADER_UNIFORM_INT);
            SetShaderValue(hdrMat.shader, GetShaderLocation(hdrMat.shader, "viewPos"),
                           &camera.position, SHADER_UNIFORM_VEC3);
            DrawModel(hdrSkybox, camera.position, 1.0f, WHITE);
            rlEnableBackfaceCulling();
            rlEnableDepthMask();
            EndMode3D();
        }

        if (IsTextureValid(cubemapTexture) && previewTarget == PreviewTarget::CUBEMAP) {
            BeginMode3D(camera);
            // Draw skybox
            rlDisableDepthMask();
            rlDisableBackfaceCulling();
            SetShaderValue(skyboxMat.shader, GetShaderLocation(skyboxMat.shader, "viewPos"),
                           &camera.position, SHADER_UNIFORM_VEC3);
            int value = skyboxFlipY ? 1 : 0;
            SetShaderValue(skyboxMat.shader, GetShaderLocation(skyboxMat.shader, "flipY"),
                           &value, SHADER_UNIFORM_INT);
            DrawModel(skybox, camera.position, 1.0f, WHITE);
            rlEnableBackfaceCulling();
            rlEnableDepthMask();
            EndMode3D();
        }


        DrawFPS(windowWidth - 80, 0);


        Rectangle rect = {Editor::MediumGap(), Editor::MediumGap(), 400, Editor::TextSizeF() * 1.5f};
        DrawRectangle(0, 0, rect.width + 3 * Editor::MediumGap(), settingsHeight, Fade(BLACK, .5f));

        GuiToggleGroup({rect.x, rect.y, rect.width / 3, rect.height}, "HDR;Cubemap;Irradiance",
                       reinterpret_cast<int *>(&previewTarget));
        rect.y += rect.height + Editor::SmallGap();

        GuiLabel(rect, TextFormat("HDR: %s, Cubemap: %s, Irradiance: %s",
                                  IsTextureValid(hdrTexture) ? "Loaded" : "Not loaded",
                                  IsTextureValid(cubemapTexture) ? "Loaded" : "Not loaded",
                                  IsTextureValid(irradianceTexture) ? "Loaded" : "Not loaded"));
        rect.y += rect.height + Editor::SmallGap();

        GuiToggleGroup({rect.x, rect.y, rect.width / 5, rect.height}, dimensionNames.c_str(), &dimensionIndex);
        rect.y += rect.height + Editor::SmallGap();

        if (GuiButton(rect, "HDR to cubemap")) {
            if (!IsTextureValid(hdrTexture)) {
                statusText = "No HDR texture loaded";
                statusWarning = true;
            } else {
                renderTexture = LoadRenderTexture(dimensions[dimensionIndex], dimensions[dimensionIndex]);
                camera.fovy = 90.0f;
                camera.position = Vector3{0.0f, 0.0f, 0.0f};
                for (int i = 0; i < 6; i++) {
                    camera.target = cameraTargets[i];
                    camera.up = cameraUps[i];

                    RenderCubeFace(i, renderTexture, hdrSkybox, hdrMat, camera, skyboxFlipY);

                    // export
                    Image image = LoadImageFromTexture(renderTexture.texture);
                    std::string path = TextFormat("%s/textures/cubemap%i.png", INTERNAL_ASSET_DIR, i);
                    if (!FileExists(path.c_str())) {
                        Utils::CreateEmptyFile(path);
                    }
                    ExportImage(image, path.c_str());
                    UnloadImage(image);
                }

                GenerateCubemapAtlas(renderTexture.texture.width, renderTexture.texture.height);
                LoadCubemap(INTERNAL_ASSET_DIR "/textures/cubemap.png", cubemapTexture, skyboxMat);

                statusText = "Cubemap generated";
                statusWarning = false;
            }
        }
        rect.y += rect.height + Editor::SmallGap();
        Editor::DrawStatusInfoBox(rect, statusText, statusWarning);

        GuiCheckBox({rect.x, rect.y, rect.height, rect.height}, "Flip Y", &skyboxFlipY);
        rect.y += rect.height + Editor::SmallGap();

        GuiToggleGroup({rect.x, rect.y, rect.width / 6, rect.height}, targetNames.c_str(), &targetFace);
        rect.y += rect.height + Editor::SmallGap();

        if (GuiButton(rect, "Set Target Face")) {
            camera.fovy = 90.0f;
            camera.position = Vector3{0.0f, 0.0f, 0.0f};
            camera.target = cameraTargets[targetFace];
            camera.up = cameraUps[targetFace];
            std::cout << "Camera target: " << cameraTargets[targetFace].x << ", "
                    << cameraTargets[targetFace].y << ", " << cameraTargets[targetFace].z << std::endl;
            std::cout << "Up: " << cameraUps[targetFace].x << ", "
                    << cameraUps[targetFace].y << ", " << cameraUps[targetFace].z << std::endl;
        }
        rect.y += rect.height + Editor::SmallGap();

        settingsHeight = rect.y;
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
