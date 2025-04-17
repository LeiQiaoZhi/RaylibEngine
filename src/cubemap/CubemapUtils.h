//
// Created by Qiaozhi Lei on 4/15/25.
//

#ifndef CUBEMAPUTILS_H
#define CUBEMAPUTILS_H

#include <raylib.h>
#include "raymath.h"
#include "rlgl.h"

inline std::string targetNames = "X+;X-;Y+;Y-;Z+;Z-";

inline std::string dimensionNames = "256;512;1024;2048;4096";
inline int dimensions[5] = {256, 512, 1024, 2048, 4096};

inline Vector3 cameraTargets[6] = {
    (Vector3){1.0f, 0.0f, 0.0f},
    (Vector3){-1.0f, 0.0f, 0.0f},
    (Vector3){0.0f, 1.0f, 0.0f},
    (Vector3){0.0f, -1.0f, 0.0f},
    (Vector3){0.0f, 0.0f, 1.0f},
    (Vector3){0.0f, 0.0f, -1.0f},
};

inline Vector3 cameraUps[6] = {
    (Vector3){0.0f, 1.0f, 0.0f},
    (Vector3){0.0f, 1.0f, 0.0f},
    (Vector3){-1.0f, 0.0f, 0.0f},
    (Vector3){1.0f, 0.0f, 0.0f},
    (Vector3){0.0f, 1.0f, 0.0f},
    (Vector3){0.0f, 1.0f, 0.0f}
};

inline Vector2 atlasPositions[6] = {
    (Vector2){1.0f, 1.0f},
    (Vector2){3.0f, 1.0f},
    (Vector2){1.0f, 2.0f},
    (Vector2){1.0f, 0.0f},
    (Vector2){2.0f, 1.0f},
    (Vector2){0.0f, 1.0f}
};

inline void LoadHDRMap(const std::string &path, Texture &hdrTexture, Material &hdrMat) {
    std::cout << "Dropped hdr file: " << path << std::endl;
    UnloadTexture(hdrTexture);
    hdrTexture = LoadTexture(path.c_str());

    hdrMat.maps[MATERIAL_MAP_DIFFUSE].texture = hdrTexture;
}

inline void LoadCubemap(const std::string &path, TextureCubemap &cubemapTexture, Material &skyboxMat) {
    std::cout << "Dropped png file: " << path << std::endl;
    UnloadTexture(cubemapTexture);
    Image cubemapImage = LoadImage(path.c_str());
    cubemapTexture = LoadTextureCubemap(cubemapImage, CUBEMAP_LAYOUT_AUTO_DETECT);
    UnloadImage(cubemapImage);

    skyboxMat.maps[MATERIAL_MAP_CUBEMAP].texture = cubemapTexture;
}

inline void GenerateCubemapAtlas(int width, int height) {
    int atlasWidth = width * 4;
    int atlasHeight = height * 3;
    Image atlas = GenImageColor(atlasWidth, atlasHeight, BLACK); // blank atlas
    std::array<Image, 6> cubeImages;
    for (int i = 0; i < 6; i++) {
        std::string path = TextFormat("%s/textures/cubemap%i.png", INTERNAL_ASSET_DIR, i);
        cubeImages[i] = LoadImage(path.c_str());
    }
    Rectangle srcRect = {0, 0, static_cast<float>(width), static_cast<float>(height)};

    for (int i = 0; i < 6; i++) {
        Rectangle destRect = {
            atlasPositions[i].x * width,
            atlasPositions[i].y * height,
            srcRect.width,
            srcRect.height
        };
        ImageDraw(&atlas, cubeImages[i], srcRect, destRect, WHITE);
    }

    ExportImage(atlas, (std::string(INTERNAL_ASSET_DIR) + "/textures/cubemap.png").c_str());

    for (int i = 0; i < 6; i++) {
        UnloadImage(cubeImages[i]);
    }
}

inline void RenderCubeFace(int faceId, const RenderTexture &renderTexture, const Model &model, const Material &material,
                           const Camera &camera, const bool flipY = false) {
    BeginTextureMode(renderTexture);
    BeginMode3D(camera);
    rlDisableDepthMask();
    rlDisableBackfaceCulling();
    int value = flipY ? 0 : 1; // invert to match coordinate system change
    SetShaderValue(material.shader, GetShaderLocation(material.shader, "flipY"),
                   &value, SHADER_UNIFORM_INT);
    SetShaderValue(material.shader, GetShaderLocation(material.shader, "viewPos"),
                   &camera.position, SHADER_UNIFORM_VEC3);
    DrawModel(model, camera.position, 1.0f, WHITE);
    rlEnableBackfaceCulling();
    rlEnableDepthMask();
    EndMode3D();
    EndTextureMode();
}

// Generate cubemap texture from HDR texture
inline TextureCubemap GenTextureCubemap(Shader shader, Texture2D panorama, int size, int format) {
    TextureCubemap cubemap = {0};

    rlDisableBackfaceCulling(); // Disable backface culling to render inside the cube

    // STEP 1: Setup framebuffer
    //------------------------------------------------------------------------------------------
    unsigned int rbo = rlLoadTextureDepth(size, size, true);
    cubemap.id = rlLoadTextureCubemap(0, size, format, 1);

    unsigned int fbo = rlLoadFramebuffer();
    rlFramebufferAttach(fbo, rbo, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);
    rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X, 0);

    // Check if framebuffer is complete with attachments (valid)
    if (rlFramebufferComplete(fbo)) TraceLog(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", fbo);
    //------------------------------------------------------------------------------------------

    // STEP 2: Draw to framebuffer
    //------------------------------------------------------------------------------------------
    // NOTE: Shader is used to convert HDR equirectangular environment map to cubemap equivalent (6 faces)
    rlEnableShader(shader.id);

    // Define projection matrix and send it to shader
    Matrix matFboProjection = MatrixPerspective(90.0 * DEG2RAD, 1.0, rlGetCullDistanceNear(), rlGetCullDistanceFar());
    rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_PROJECTION], matFboProjection);

    // Define view matrix for every side of the cubemap
    Matrix fboViews[6] = {
        MatrixLookAt((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){1.0f, 0.0f, 0.0f}, (Vector3){0.0f, -1.0f, 0.0f}),
        MatrixLookAt((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){-1.0f, 0.0f, 0.0f}, (Vector3){0.0f, -1.0f, 0.0f}),
        MatrixLookAt((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){0.0f, 1.0f, 0.0f}, (Vector3){0.0f, 0.0f, 1.0f}),
        MatrixLookAt((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){0.0f, -1.0f, 0.0f}, (Vector3){0.0f, 0.0f, -1.0f}),
        MatrixLookAt((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){0.0f, 0.0f, 1.0f}, (Vector3){0.0f, -1.0f, 0.0f}),
        MatrixLookAt((Vector3){0.0f, 0.0f, 0.0f}, (Vector3){0.0f, 0.0f, -1.0f}, (Vector3){0.0f, -1.0f, 0.0f})
    };

    rlViewport(0, 0, size, size); // Set viewport to current fbo dimensions

    // Activate and enable texture for drawing to cubemap faces
    rlActiveTextureSlot(0);
    rlEnableTexture(panorama.id);

    for (int i = 0; i < 6; i++) {
        std::cout << "Cubemap face: " << i << std::endl;
        // Set the view matrix for the current cube face
        rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_VIEW], fboViews[i]);

        // Select the current cubemap face attachment for the fbo
        // WARNING: This function by default enables->attach->disables fbo!!!
        rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X + i, 0);
        rlEnableFramebuffer(fbo);

        // Load and draw a cube, it uses the current enabled texture
        rlClearScreenBuffers();
        rlLoadDrawCube();

        // ALTERNATIVE: Try to use internal batch system to draw the cube instead of rlLoadDrawCube
        // for some reason this method does not work, maybe due to cube triangles definition? normals pointing out?
        // TODO: Investigate this issue...
        //rlSetTexture(panorama.id); // WARNING: It must be called after enabling current framebuffer if using internal batch system!
        //rlClearScreenBuffers();
        //DrawCubeV(Vector3Zero(), Vector3One(), WHITE);
        //rlDrawRenderBatchActive();
    }
    //------------------------------------------------------------------------------------------

    // STEP 3: Unload framebuffer and reset state
    //------------------------------------------------------------------------------------------
    rlDisableShader(); // Unbind shader
    rlDisableTexture(); // Unbind texture
    rlDisableFramebuffer(); // Unbind framebuffer
    rlUnloadFramebuffer(fbo); // Unload framebuffer (and automatically attached depth texture/renderbuffer)

    // Reset viewport dimensions to default
    rlViewport(0, 0, rlGetFramebufferWidth(), rlGetFramebufferHeight());
    rlEnableBackfaceCulling();
    //------------------------------------------------------------------------------------------

    cubemap.width = size;
    cubemap.height = size;
    cubemap.mipmaps = 1;
    cubemap.format = format;

    std::cout << "Cubemap generated with size: " << cubemap.width << "x" << cubemap.height << std::endl;

    return cubemap;
}

#endif //CUBEMAPUTILS_H
