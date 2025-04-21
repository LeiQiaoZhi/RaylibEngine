#include "raygui.h"

#define RAYGUI_IMPLEMENTATION
#include <string>
#include "raygui.h"
#undef RAYGUI_IMPLEMENTATION

#include "raylib.h"
#include "../common/editor/Editor.h"
#include "sol/sol.hpp"
#include "CubemapUtils.h"
#include "../common/editor/DropdownProperty.h"
#include "../common/editor/FoldoutProperty.h"
#include "../common/utils/RaylibUtils.h"
#include "../common/utils/Utils.h"

enum class PreviewTarget {
    HDR = 0,
    CUBEMAP,
    IRRADIANCE,
    PREFILTER
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
    const std::string windowName = "Cubemap Tool";
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
    Texture2D cubemapPreviewTexture = Texture();
    TextureCubemap cubemapTexture = TextureCubemap();
    Texture2D irradiancePreviewTexture = Texture();
    TextureCubemap irradianceTexture = TextureCubemap();
    Texture2D prefilterPreviewTexture = Texture();
    TextureCubemap prefilterTexture = TextureCubemap();

    Camera camera;
    camera.fovy = 90.0;
    camera.position = {0.0f, 0.0f, 0.0f};
    camera.target = {1.0f, 0.0f, 0.0f};
    camera.up = {0.0f, 1.0f, 0.0f};
    camera.projection = CAMERA_PERSPECTIVE;

    Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
    Model hdrSkybox = LoadModelFromMesh(cube);
    Material &hdrMat = hdrSkybox.materials[hdrSkybox.meshMaterial[0]];
    hdrMat.shader = LoadShader(
        INTERNAL_ASSET_DIR "/shaders/default.vert",
        INTERNAL_ASSET_DIR "/shaders/cubemap.frag"
    );
    hdrMat.shader.locs[SHADER_LOC_MAP_DIFFUSE] = GetShaderLocation(hdrMat.shader, "equirectangularMap");

    Model skybox = LoadModelFromMesh(cube);
    Material &skyboxMat = skybox.materials[skybox.meshMaterial[0]];
    skyboxMat.shader = LoadShader(
        ASSET_DIR "/shaders/custom.vert",
        ASSET_DIR "/shaders/skybox.frag"
    );
    skyboxMat.shader.locs[SHADER_LOC_MAP_CUBEMAP] = GetShaderLocation(skyboxMat.shader, "environmentMap");

    Model irradiancePreviewSkybox = LoadModelFromMesh(cube);
    Material &irradiancePreviewSkyboxMat = irradiancePreviewSkybox.materials[irradiancePreviewSkybox.meshMaterial[0]];
    irradiancePreviewSkyboxMat.shader = skyboxMat.shader;

    Model irradianceSkybox = LoadModelFromMesh(cube);
    Material &irradianceMat = irradianceSkybox.materials[irradianceSkybox.meshMaterial[0]];
    irradianceMat.shader = LoadShader(
        INTERNAL_ASSET_DIR "/shaders/default.vert",
        INTERNAL_ASSET_DIR "/shaders/irradiance.frag"
    );
    irradianceMat.shader.locs[SHADER_LOC_MAP_CUBEMAP] = GetShaderLocation(irradianceMat.shader, "environmentMap");

    Model prefilterSkybox = LoadModelFromMesh(cube);
    Material &prefilterMat = prefilterSkybox.materials[prefilterSkybox.meshMaterial[0]];
    prefilterMat.shader = LoadShader(
        INTERNAL_ASSET_DIR "/shaders/default.vert",
        INTERNAL_ASSET_DIR "/shaders/prefilter.frag"
    );
    prefilterMat.shader.locs[SHADER_LOC_MAP_CUBEMAP] = GetShaderLocation(prefilterMat.shader, "environmentMap");

    Model prefilterPreviewSkybox = LoadModelFromMesh(cube);
    Material &prefilterPreviewSkyboxMat = prefilterPreviewSkybox.materials[prefilterPreviewSkybox.meshMaterial[0]];
    prefilterPreviewSkyboxMat.shader = skyboxMat.shader;

    std::vector<Material *> cubemapDependentMaterials = {
        &skyboxMat,
        &irradianceMat,
        &prefilterMat
    };

    RenderTexture renderTexture;

    bool skyboxFlipY = true;
    bool updateCamera = false;
    int targetFace = 0;
    PreviewTarget previewTarget = PreviewTarget::HDR;
    int dimensionIndex = 2;

    // ui states
    int currentMip = 0;
    constexpr int totalMipLevels = 5;
    float settingsHeight = 0.0f;
    std::string statusText;
    bool statusWarning = false;
    std::string irradianceStatusText;
    bool irradianceStatusWarning = false;
    FoldoutProperty debugFoldout = FoldoutProperty("Debug", true);

    // default file
    LoadHDRMap(INTERNAL_ASSET_DIR "/textures/default.hdr", hdrTexture, hdrMat, irradianceMat);
    LoadCubemap(INTERNAL_ASSET_DIR "/textures/cubemap.png", cubemapTexture, cubemapDependentMaterials,
                cubemapPreviewTexture);

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
            if (droppedFiles.count == 1 && IsFileExtension(droppedFiles.paths[0], ".png")) {
                LoadCubemap(droppedFiles.paths[0], cubemapTexture, cubemapDependentMaterials, cubemapPreviewTexture);
            }
            if (droppedFiles.count == 1 && IsFileExtension(droppedFiles.paths[0], ".hdr"))
                LoadHDRMap(droppedFiles.paths[0], hdrTexture, hdrMat, irradianceMat);
            UnloadDroppedFiles(droppedFiles); // Unload filepaths from memory
        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

        if (IsTextureValid(hdrTexture) && previewTarget == PreviewTarget::HDR) {
            DrawSkybox(camera, hdrSkybox, hdrMat, skyboxFlipY);
            DrawPreviewTexture(hdrTexture, windowWidth, windowHeight);
        }

        if (IsTextureValid(cubemapTexture) && previewTarget == PreviewTarget::CUBEMAP) {
            DrawSkybox(camera, skybox, skyboxMat, skyboxFlipY);
            DrawPreviewTexture(cubemapPreviewTexture, windowWidth, windowHeight);
        }

        if (IsTextureValid(irradianceTexture) && previewTarget == PreviewTarget::IRRADIANCE) {
            DrawSkybox(camera, irradiancePreviewSkybox, irradiancePreviewSkyboxMat, skyboxFlipY);
            DrawPreviewTexture(irradiancePreviewTexture, windowWidth, windowHeight);
        }

        if (IsTextureValid(prefilterTexture) && previewTarget == PreviewTarget::PREFILTER) {
            DrawSkybox(camera, prefilterPreviewSkybox, prefilterPreviewSkyboxMat, skyboxFlipY);
            DrawPreviewTexture(prefilterPreviewTexture, windowWidth, windowHeight);
        }

        DrawFPS(windowWidth - 80, 0);


        Rectangle rect = {Editor::MediumGap(), Editor::MediumGap(), 400, Editor::TextSizeF() * 1.5f};
        DrawRectangle(0, 0, rect.width + 2 * Editor::MediumGap(), settingsHeight, Fade(BLACK, .5f));

        GuiToggleGroup({rect.x, rect.y, rect.width / 4, rect.height}, "HDR;Cubemap;Irradiance;Prefilter",
                       reinterpret_cast<int *>(&previewTarget));
        rect.y += rect.height + Editor::SmallGap();

        GuiToggleGroup({rect.x, rect.y, (rect.width - Editor::SmallGap()) / dimensions.size(), rect.height},
                       dimensionNames.c_str(),
                       &dimensionIndex);
        rect.y += rect.height + Editor::SmallGap();

        if (GuiButton(rect, "HDR to cubemap")) {
            if (!IsTextureValid(hdrTexture)) {
                statusText = "No HDR texture loaded";
                statusWarning = true;
            } else {
                renderTexture = RaylibUtils::LoadRenderTextureX(dimensions[dimensionIndex], dimensions[dimensionIndex],
                                                                PIXELFORMAT_UNCOMPRESSED_R32G32B32);
                camera.position = Vector3{0.0f, 0.0f, 0.0f};
                for (int i = 0; i < 6; i++) {
                    RenderCubeFace(i, renderTexture, hdrSkybox, hdrMat, camera, skyboxFlipY);

                    std::string path = TextFormat("%s/textures/cubemap%i.png", INTERNAL_ASSET_DIR, i);
                    SaveRenderTexture(path, renderTexture);
                }

                GenerateCubemapAtlas("cubemap", renderTexture.texture.width, renderTexture.texture.height);
                LoadCubemap(INTERNAL_ASSET_DIR "/textures/cubemap.png", cubemapTexture, cubemapDependentMaterials,
                            cubemapPreviewTexture);

                statusText = "Cubemap generated";
                statusWarning = false;
            }
        }
        rect.y += rect.height + Editor::SmallGap();
        Editor::DrawStatusInfoBox(rect, statusText, statusWarning);

        if (GuiButton(rect, "Generate Irradiance")) {
            if (!IsTextureValid(cubemapTexture)) {
                irradianceStatusText = "No cubemap texture loaded";
                irradianceStatusWarning = true;
            } else {
                renderTexture = RaylibUtils::LoadRenderTextureX(dimensions[dimensionIndex], dimensions[dimensionIndex],
                                                                PIXELFORMAT_UNCOMPRESSED_R32G32B32);
                camera.position = Vector3{0.0f, 0.0f, 0.0f};
                for (int i = 0; i < 6; i++) {
                    RenderCubeFace(i, renderTexture, irradianceSkybox, irradianceMat, camera, skyboxFlipY);

                    std::string path = TextFormat("%s/textures/irradiance%i.png", INTERNAL_ASSET_DIR, i);
                    SaveRenderTexture(path, renderTexture);
                }

                GenerateCubemapAtlas("irradiance", renderTexture.texture.width, renderTexture.texture.height);
                LoadIrradianceMapForPreview(INTERNAL_ASSET_DIR "/textures/irradiance.png", irradianceTexture,
                                            irradiancePreviewSkyboxMat,
                                            irradiancePreviewTexture);

                irradianceStatusText = "Irradiance map generated";
                irradianceStatusWarning = false;
            }
        }
        rect.y += rect.height + Editor::SmallGap();
        Editor::DrawStatusInfoBox(rect, irradianceStatusText, irradianceStatusWarning);

        if (GuiButton(rect, "Generate Prefilter Maps")) {
            if (!IsTextureValid(cubemapTexture)) {
            } else {
                camera.position = Vector3{0.0f, 0.0f, 0.0f};
                for (unsigned int mip = 0; mip < totalMipLevels; ++mip) {
                    float dimension = 256 * std::pow(0.5, mip);
                    renderTexture = RaylibUtils::LoadRenderTextureX(dimension, dimension,
                                                                    PIXELFORMAT_UNCOMPRESSED_R32G32B32);

                    float roughness = static_cast<float>(mip) / static_cast<float>(totalMipLevels);
                    SetShaderValue(prefilterMat.shader, GetShaderLocation(prefilterMat.shader, "roughness"),
                                   &roughness, SHADER_UNIFORM_FLOAT);

                    for (int i = 0; i < 6; i++) {
                        RenderCubeFace(i, renderTexture, prefilterSkybox, prefilterMat, camera, skyboxFlipY);

                        std::string path = TextFormat("%s/textures/prefilter-%i%i.png", INTERNAL_ASSET_DIR, mip, i);
                        SaveRenderTexture(path, renderTexture);
                    }

                    std::string name = TextFormat("prefilter-%i", mip);
                    GenerateCubemapAtlas(name, renderTexture.texture.width, renderTexture.texture.height);
                }
                std::string name = TextFormat("prefilter-%i", currentMip);
                LoadPrefilterMapForPreview(TextFormat("%s/textures/%s.png", INTERNAL_ASSET_DIR, name.c_str()),
                                           prefilterTexture,
                                           prefilterPreviewSkyboxMat,
                                           prefilterPreviewTexture);
            }
        }
        rect.y += rect.height + Editor::SmallGap();

        GuiCheckBox({rect.x, rect.y, rect.height, rect.height}, "Flip Y", &skyboxFlipY);
        rect.y += rect.height + Editor::SmallGap();

        debugFoldout.OnEditorGUI(rect);
        if (!debugFoldout.IsFolded()) {
            GuiLabel(rect, TextFormat("HDR: %s, Cubemap: %s, Irradiance: %s",
                                      IsTextureValid(hdrTexture) ? "Loaded" : "Not loaded",
                                      IsTextureValid(cubemapTexture) ? "Loaded" : "Not loaded",
                                      IsTextureValid(irradianceTexture) ? "Loaded" : "Not loaded"));
            rect.y += rect.height + Editor::SmallGap();


            GuiToggleGroup({rect.x, rect.y, rect.width / 6, rect.height}, targetNames.c_str(), &targetFace);
            rect.y += rect.height + Editor::SmallGap();

            if (GuiButton(rect, "Set Target Face")) {
                camera.fovy = 90.0f;
                camera.position = Vector3{0.0f, 0.0f, 0.0f};
                camera.target = cameraTargets[targetFace];
                camera.up = cameraUps[targetFace];
            }
            rect.y += rect.height + Editor::SmallGap();

            GuiToggleGroup({rect.x, rect.y, rect.width / totalMipLevels, rect.height}, "M0;M1;M2;M3;M4", &currentMip);
            rect.y += rect.height + Editor::SmallGap();
            if (GuiButton(rect, "Set Prefilter Mip Level")) {
                std::cout << "Setting prefilter mip level: " << currentMip << std::endl;
                std::string name = TextFormat("prefilter-%i", currentMip);
                LoadPrefilterMapForPreview(TextFormat("%s/textures/%s.png", INTERNAL_ASSET_DIR, name.c_str()),
                                           prefilterTexture,
                                           prefilterPreviewSkyboxMat,
                                           prefilterPreviewTexture);
            }
            rect.y += rect.height + Editor::SmallGap();
        }

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
