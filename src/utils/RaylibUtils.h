#ifndef RAYLIBUTILS_H
#define RAYLIBUTILS_H

#include <config.h>

#include "raylib.h"
#include <iostream>
#include "raymath.h"
#include <nlohmann/json.hpp>
#include "JsonUtils.h"

using json = nlohmann::json;

class RaylibUtils {
public:
    static float &Vector3Get(Vector3 &v, int index) {
        switch (index) {
            case 0: return v.x;
            case 1: return v.y;
            case 2: return v.z;
            default: throw std::out_of_range("Index must be 0, 1, or 2");
        }
    }

    static float *Vector3Get(Vector3 *v, int index) {
        switch (index) {
            case 0: return &v->x;
            case 1: return &v->y;
            case 2: return &v->z;
            default: throw std::out_of_range("Index must be 0, 1, or 2");
        }
    }

    // Helper function to access the vector's i-th component
    template<typename T>
    static float *GetVectorComponent(T *v, int index) {
        if constexpr (std::is_same<T, Vector3>::value) {
            switch (index) {
                case 0: return &v->x;
                case 1: return &v->y;
                case 2: return &v->z;
                default: throw std::out_of_range("Index must be 0, 1, or 2");
            }
        }
        if constexpr (std::is_same<T, Vector2>::value) {
            switch (index) {
                case 0: return &v->x;
                case 1: return &v->y;
                default: throw std::out_of_range("Index must be 0, or 1");
            }
        }
        throw std::out_of_range("Type must be vector3 or 2");
    }

    static void DrawDebugGrid(const Vector3 &position, const Vector3 &right, const Vector3 &forward,
                              const Vector2 &size,
                              const Vector2 &spacing, const Color &color) {
        // prevent division by zero
        if (spacing.x == 0 || spacing.y == 0 || size.x == 0 || size.y == 0) {
            return;
        }

        const Vector3 bottomLeft = position - right * size.x / 2 - forward * size.y / 2;

        for (int i = 0; i <= size.x / spacing.x; i++) {
            const Vector3 start = bottomLeft + right * i * spacing.x;
            const Vector3 end = start + forward * size.y;
            DrawLine3D(start, end, color);
        }
        for (int i = 0; i <= size.y / spacing.y; i++) {
            const Vector3 start = bottomLeft + forward * i * spacing.y;
            const Vector3 end = start + right * size.x;
            DrawLine3D(start, end, color);
        }
    }

    static Vector3 GetScaleFromTransform(const Matrix &mat) {
        return {
            Vector3Length(Vector3{mat.m0, mat.m1, mat.m2}),
            Vector3Length(Vector3{mat.m4, mat.m5, mat.m6}),
            Vector3Length(Vector3{mat.m8, mat.m9, mat.m10})
        };
    }

    // Compute model bounding box limits (considers all meshes)
    static void DrawModelBoundingBoxAfterTransform(Model model, Color color) {
        BoundingBox bbox = {0};

        if (model.meshCount > 0) {
            Vector3 temp = {0};
            bbox = GetMeshBoundingBox(model.meshes[0]);

            for (int i = 1; i < model.meshCount; i++) {
                BoundingBox tempBounds = GetMeshBoundingBox(model.meshes[i]);

                temp.x = (bbox.min.x < tempBounds.min.x) ? bbox.min.x : tempBounds.min.x;
                temp.y = (bbox.min.y < tempBounds.min.y) ? bbox.min.y : tempBounds.min.y;
                temp.z = (bbox.min.z < tempBounds.min.z) ? bbox.min.z : tempBounds.min.z;
                bbox.min = temp;

                temp.x = (bbox.max.x > tempBounds.max.x) ? bbox.max.x : tempBounds.max.x;
                temp.y = (bbox.max.y > tempBounds.max.y) ? bbox.max.y : tempBounds.max.y;
                temp.z = (bbox.max.z > tempBounds.max.z) ? bbox.max.z : tempBounds.max.z;
                bbox.max = temp;
            }
        }

        Vector3 corners[8] = {
            {bbox.min.x, bbox.min.y, bbox.min.z},
            {bbox.min.x, bbox.min.y, bbox.max.z},
            {bbox.min.x, bbox.max.y, bbox.min.z},
            {bbox.min.x, bbox.max.y, bbox.max.z},
            {bbox.max.x, bbox.min.y, bbox.min.z},
            {bbox.max.x, bbox.min.y, bbox.max.z},
            {bbox.max.x, bbox.max.y, bbox.min.z},
            {bbox.max.x, bbox.max.y, bbox.max.z}
        };

        // Apply transformation to all corners
        for (int i = 0; i < 8; i++) {
            corners[i] = Vector3Transform(corners[i], model.transform);
        }

        DrawLine3D(corners[0], corners[1], color);
        DrawLine3D(corners[1], corners[3], color);
        DrawLine3D(corners[3], corners[2], color);
        DrawLine3D(corners[2], corners[0], color);

        DrawLine3D(corners[4], corners[5], color);
        DrawLine3D(corners[5], corners[7], color);
        DrawLine3D(corners[7], corners[6], color);
        DrawLine3D(corners[6], corners[4], color);

        DrawLine3D(corners[0], corners[4], color);
        DrawLine3D(corners[1], corners[5], color);
        DrawLine3D(corners[2], corners[6], color);
        DrawLine3D(corners[3], corners[7], color);
    }

    static std::string MaterialMapTypeToString(const int i) {
        switch (i) {
            case MATERIAL_MAP_ALBEDO: return "Albedo";
            case MATERIAL_MAP_METALNESS: return "Metalness";
            case MATERIAL_MAP_NORMAL: return "Normal";
            case MATERIAL_MAP_ROUGHNESS: return "Roughness";
            case MATERIAL_MAP_OCCLUSION: return "Occlusion";
            case MATERIAL_MAP_EMISSION: return "Emission";
            case MATERIAL_MAP_HEIGHT: return "Height";
            case MATERIAL_MAP_CUBEMAP: return "Cubemap";
            case MATERIAL_MAP_IRRADIANCE: return "Irradiance";
            case MATERIAL_MAP_PREFILTER: return "Prefilter";
            case MATERIAL_MAP_BRDF: return "BRDF";
            default: return "Unknown";
        }
    }


    static Material LoadMaterialFromFile(const char *path) {
        json j = JsonUtils::JsonFromFile(path);
        Material material = LoadMaterialDefault();

        // Shader
        if (j["Shader"]["Vertex"] != "default") {
            std::string vertPath = ASSET_DIR + std::string(j["Shader"]["Vertex"]);
        }
        if (j["Shader"]["Fragment"] != "default") {
            std::string fragPath = ASSET_DIR + std::string(j["Shader"]["Fragment"]);
        }

        // Textures
        auto &mapsArray = j["Maps"];
        for (int i = 0; i < std::min(MAX_MATERIAL_MAPS, static_cast<int>(mapsArray.size())); i++) {
            auto &map = mapsArray[i];
            auto &color = map["Color"];
            const int type = map["Type"];
            std::string texturePath = ASSET_DIR + std::string(map["Path"]);
            material.maps[type].texture = LoadTexture(texturePath.c_str());
            material.maps[type].color = Color{color[0], color[1], color[2], color[3]};
        }

        // TODO: params

        return material;
    }

    static Mesh GenCubeCustomResolution(int size[3], Vector3 sizeWorld) {
        Mesh mesh = {0};

        // vertices along x, y, z
        int verticesCount = 2 * (size[0] * size[1] + size[1] * size[2] + size[2] * size[0]);
        int trianglesCount = 4 * ((size[0] - 1) * (size[1] - 1) + (size[1] - 1) * (size[2] - 1) + (
                                      size[2] - 1) * (size[0] - 1));
        Vector3 *vertices = static_cast<Vector3 *>(RL_MALLOC(verticesCount*sizeof(Vector3)));
        Vector3 *normals = static_cast<Vector3 *>(RL_MALLOC(verticesCount*sizeof(Vector3)));
        int *indices = static_cast<int *>(RL_MALLOC(trianglesCount*3*sizeof(int)));
        int verticesAdded = 0;
        int verticesPerPlane = size[0] * size[2];
        // bottom and top
        for (int x = 0; x < size[0]; x++) {
            for (int z = 0; z < size[2]; z++) {
                // Bottom
                vertices[x * size[2] + z + verticesAdded] = {
                    x * sizeWorld.x / (size[0] - 1),
                    0,
                    z * sizeWorld.z / (size[2] - 1)
                };
                normals[x * size[2] + z + verticesAdded] = {0, -1, 0};

                // Top
                vertices[x * size[2] + z + verticesPerPlane + verticesAdded] = {
                    x * sizeWorld.x / (size[0] - 1),
                    sizeWorld.y,
                    z * sizeWorld.z / (size[2] - 1)
                };
                normals[x * size[2] + z + verticesPerPlane + verticesAdded] = {0, 1, 0};
            }
        }

        int index = 0;
        for (int x = 0; x < size[0] - 1; x++) {
            for (int z = 0; z < size[2] - 1; z++) {
                // Bottom
                int bottomLeftIndex = x * size[2] + z;
                int bottomRightIndex = bottomLeftIndex + size[2];
                int topLeftIndex = bottomLeftIndex + 1;
                int topRightIndex = bottomRightIndex + 1;
                indices[index++] = bottomLeftIndex;
                indices[index++] = bottomRightIndex;
                indices[index++] = topLeftIndex;
                indices[index++] = topLeftIndex;
                indices[index++] = bottomRightIndex;
                indices[index++] = topRightIndex;
                // Top
                bottomLeftIndex += verticesPerPlane;
                bottomRightIndex += verticesPerPlane;
                topLeftIndex += verticesPerPlane;
                topRightIndex += verticesPerPlane;
                indices[index++] = bottomLeftIndex;
                indices[index++] = topLeftIndex;
                indices[index++] = bottomRightIndex;
                indices[index++] = topLeftIndex;
                indices[index++] = topRightIndex;
                indices[index++] = bottomRightIndex;
            }
        }

        // left and right
        verticesAdded += verticesPerPlane * 2;
        verticesPerPlane = size[1] * size[2];
        for (int y = 0; y < size[1]; y++) {
            for (int z = 0; z < size[2]; z++) {
                // Left
                vertices[y * size[2] + z + verticesAdded] = {
                    0,
                    y * sizeWorld.y / (size[1] - 1),
                    z * sizeWorld.z / (size[2] - 1)
                };
                normals[y * size[2] + z + verticesAdded] = {-1, 0, 0};

                // Right
                vertices[y * size[2] + z + verticesPerPlane + verticesAdded] = {
                    sizeWorld.x,
                    y * sizeWorld.y / (size[1] - 1),
                    z * sizeWorld.z / (size[2] - 1)
                };
                normals[y * size[2] + z + verticesPerPlane + verticesAdded] = {1, 0, 0};
            }
        }

        for (int y = 0; y < size[1] - 1; y++) {
            for (int z = 0; z < size[2] - 1; z++) {
                // Left
                int bottomLeftIndex = y * size[2] + z + verticesAdded;
                int bottomRightIndex = bottomLeftIndex + size[2];
                int topLeftIndex = bottomLeftIndex + 1;
                int topRightIndex = bottomRightIndex + 1;
                indices[index++] = bottomLeftIndex;
                indices[index++] = topLeftIndex;
                indices[index++] = bottomRightIndex;
                indices[index++] = topLeftIndex;
                indices[index++] = topRightIndex;
                indices[index++] = bottomRightIndex;
                // Right
                bottomLeftIndex += verticesPerPlane;
                bottomRightIndex += verticesPerPlane;
                topLeftIndex += verticesPerPlane;
                topRightIndex += verticesPerPlane;
                indices[index++] = bottomLeftIndex;
                indices[index++] = bottomRightIndex;
                indices[index++] = topLeftIndex;
                indices[index++] = topLeftIndex;
                indices[index++] = bottomRightIndex;
                indices[index++] = topRightIndex;
            }
        }


        // front and back
        verticesAdded += verticesPerPlane * 2;
        verticesPerPlane = size[0] * size[1];
        for (int x = 0; x < size[0]; x++) {
            for (int y = 0; y < size[1]; y++) {
                // Front
                vertices[x * size[1] + y + verticesAdded] = {
                    x * sizeWorld.x / (size[0] - 1),
                    y * sizeWorld.y / (size[1] - 1),
                    0,
                };
                normals[x * size[1] + y + verticesAdded] = {0, 0, -1};

                // Right
                vertices[x * size[1] + y + verticesPerPlane + verticesAdded] = {
                    x * sizeWorld.x / (size[0] - 1),
                    y * sizeWorld.y / (size[1] - 1),
                    sizeWorld.z,
                };
                normals[x * size[1] + y + verticesPerPlane + verticesAdded] = {0, 0, 1};
            }
        }

        for (int x = 0; x < size[0] - 1; x++) {
            for (int y = 0; y < size[1] - 1; y++) {
                // Front
                int bottomLeftIndex = x * size[1] + y + verticesAdded;
                int bottomRightIndex = bottomLeftIndex + size[1];
                int topLeftIndex = bottomLeftIndex + 1;
                int topRightIndex = bottomRightIndex + 1;
                indices[index++] = bottomLeftIndex;
                indices[index++] = topLeftIndex;
                indices[index++] = bottomRightIndex;
                indices[index++] = topLeftIndex;
                indices[index++] = topRightIndex;
                indices[index++] = bottomRightIndex;
                // Back
                bottomLeftIndex += verticesPerPlane;
                bottomRightIndex += verticesPerPlane;
                topLeftIndex += verticesPerPlane;
                topRightIndex += verticesPerPlane;
                indices[index++] = bottomLeftIndex;
                indices[index++] = bottomRightIndex;
                indices[index++] = topLeftIndex;
                indices[index++] = topLeftIndex;
                indices[index++] = bottomRightIndex;
                indices[index++] = topRightIndex;
            }
        }


        mesh.vertexCount = verticesCount;
        mesh.triangleCount = trianglesCount;
        mesh.vertices = (float *) RL_MALLOC(mesh.vertexCount*3*sizeof(float));
        mesh.texcoords = (float *) RL_MALLOC(mesh.vertexCount*2*sizeof(float));
        mesh.normals = (float *) RL_MALLOC(mesh.vertexCount*3*sizeof(float));
        mesh.indices = (unsigned short *) RL_MALLOC(mesh.triangleCount*3*sizeof(unsigned short));

        // Mesh vertices position array
        for (int i = 0; i < mesh.vertexCount; i++) {
            mesh.vertices[3 * i] = vertices[i].x - sizeWorld.x / 2;
            mesh.vertices[3 * i + 1] = vertices[i].y - sizeWorld.y / 2;
            mesh.vertices[3 * i + 2] = vertices[i].z - sizeWorld.z / 2;
        }

        // Mesh texcoords array
        // for (int i = 0; i < mesh.vertexCount; i++) {
        //     mesh.texcoords[2 * i] = texcoords[i].x;
        //     mesh.texcoords[2 * i + 1] = texcoords[i].y;
        // }

        // Mesh normals array
        for (int i = 0; i < mesh.vertexCount; i++) {
            mesh.normals[3 * i] = normals[i].x;
            mesh.normals[3 * i + 1] = normals[i].y;
            mesh.normals[3 * i + 2] = normals[i].z;
        }

        // Mesh indices array initialization
        for (int i = 0; i < mesh.triangleCount * 3; i++) mesh.indices[i] = indices[i];

        RL_FREE(vertices);
        RL_FREE(normals);
        // RL_FREE(texcoords);
        RL_FREE(indices);

        UploadMesh(&mesh, false);

        return mesh;
    }
};

#endif //RAYLIBUTILS_H
