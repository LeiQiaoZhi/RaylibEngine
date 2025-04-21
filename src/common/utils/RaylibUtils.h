#ifndef RAYLIBUTILS_H
#define RAYLIBUTILS_H

#include <config.h>

#include "raylib.h"
#include <iostream>
#include "raymath.h"
#include <nlohmann/json.hpp>
#include "JsonUtils.h"
#include "rlgl.h"
#include "ShaderParam.h"

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

    // Compute model bounding box limits (considers all meshes)
    static BoundingBox GetModelAABBAfterTransform(const Model &model) {
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

        // Get new AABB limits
        BoundingBox result = {corners[0], corners[0]};
        for (int i = 1; i < 8; i++) {
            result.min.x = fminf(result.min.x, corners[i].x);
            result.min.y = fminf(result.min.y, corners[i].y);
            result.min.z = fminf(result.min.z, corners[i].z);
            result.max.x = fmaxf(result.max.x, corners[i].x);
            result.max.y = fmaxf(result.max.y, corners[i].y);
            result.max.z = fmaxf(result.max.z, corners[i].z);
        }

        return result;
    }

    static RayCollision GetRayCollisionModel(const Ray &ray, const Model &model, int *closestMeshIndex = nullptr) {
        RayCollision result = {0};
        // check AABB
        const BoundingBox aabb = GetModelAABBAfterTransform(model);
        const RayCollision aabbCollision = GetRayCollisionBox(ray, aabb);

        if (!aabbCollision.hit) return result;

        // check all meshes, return closest
        for (int i = 0; i < model.meshCount; i++) {
            const Mesh mesh = model.meshes[i];
            const RayCollision meshCollision = GetRayCollisionMesh(ray, mesh, model.transform);
            if (meshCollision.hit) {
                if (!result.hit || meshCollision.distance < result.distance) {
                    result = meshCollision;
                    if (closestMeshIndex) *closestMeshIndex = i;
                }
            }
        }
        return result;
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

    static Material LoadMaterialFromFile(const char *path, std::vector<ShaderParam> &shaderParams) {
        json j = JsonUtils::JsonFromFile(path);
        Material material = LoadMaterialDefault();

        std::cout << "Loading material from file: " << path << std::endl;

        // Shader
        if (j["Shader"]["Vertex"] != "default" || j["Shader"]["Fragment"] != "default") {
            const std::string vertPath = ASSET_DIR + std::string("/shaders/") + std::string(j["Shader"]["Vertex"]);
            const std::string fragPath = ASSET_DIR + std::string("/shaders/") + std::string(j["Shader"]["Fragment"]);
            Shader shader = LoadShader(vertPath.c_str(), fragPath.c_str());

            if (IsShaderValid(shader)) {
                material.shader = shader;
            } else {
                std::cerr << "Failed to load shader: " << vertPath << " " << fragPath << std::endl;
            }

            // set up locations
            material.shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(material.shader, "viewPos");
            material.shader.locs[SHADER_LOC_MAP_ALBEDO] = GetShaderLocation(material.shader, "albedoMap");
            material.shader.locs[SHADER_LOC_COLOR_DIFFUSE] = GetShaderLocation(material.shader, "albedoColor");
            material.shader.locs[SHADER_LOC_MAP_ROUGHNESS] = GetShaderLocation(material.shader, "roughnessMap");
            material.shader.locs[SHADER_LOC_MAP_METALNESS] = GetShaderLocation(material.shader, "metallicMap");
            material.shader.locs[SHADER_LOC_MAP_OCCLUSION] = GetShaderLocation(material.shader, "occlusionMap");
            material.shader.locs[SHADER_LOC_MAP_NORMAL] = GetShaderLocation(material.shader, "normalMap");
            material.shader.locs[SHADER_LOC_MAP_HEIGHT] = GetShaderLocation(material.shader, "heightMap");
            material.shader.locs[SHADER_LOC_MAP_CUBEMAP] = GetShaderLocation(material.shader, "environmentMap");
        }

        // Textures
        auto &mapsArray = j["Maps"];
        for (int i = 0; i < std::min(MAX_MATERIAL_MAPS, static_cast<int>(mapsArray.size())); i++) {
            auto &mapJson = mapsArray[i];

            const int type = mapJson["Type"];
            std::string texturePath = ASSET_DIR + std::string(mapJson["Path"]);

            // Load texture
            if (type == MATERIAL_MAP_CUBEMAP) {
                Image image = LoadImage(texturePath.c_str());
                material.maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(image, CUBEMAP_LAYOUT_AUTO_DETECT);
                UnloadImage(image);
            } else {
                material.maps[type].texture = LoadTexture(texturePath.c_str());
            }

            // TODO: configure
            GenTextureMipmaps(&material.maps[type].texture);

            // Set texture parameters
            if (mapJson.contains("Wrap")) {
                const TextureWrap wrap = StringToTextureWrapEnum(mapJson["Wrap"]);
                SetTextureWrap(material.maps[type].texture, wrap);
            }
            if (mapJson.contains("Filter")) {
                const TextureFilter filter = StringToTextureFilterEnum(mapJson["Filter"]);
                SetTextureFilter(material.maps[type].texture, filter);
            }
            // Other map properties
            if (mapJson.contains("Color")) {
                auto &color = mapJson["Color"];
                material.maps[type].color = Color{color[0], color[1], color[2], color[3]};
            } else {
                material.maps[type].color = WHITE;
            }

            if (type == MATERIAL_MAP_NORMAL) {
                int loc = GetShaderLocation(material.shader, "useNormalMap");
                bool useNormalMap = mapJson.value("Enabled", true);
                int value = useNormalMap ? 1 : 0;
                SetShaderValue(material.shader, loc, &value, SHADER_UNIFORM_INT);
            }
        }

        //  params
        shaderParams.clear();
        for (auto &paramJson: j["Params"]) {
            ShaderParam param = {};
            param.name = paramJson["Name"];
            param.type = StringToShaderUniformDataType(paramJson["Type"]);
            if (param.type == SHADER_UNIFORM_VEC4) {
                param.value = Vector4{
                    paramJson["Value"][0], paramJson["Value"][1],
                    paramJson["Value"][2], paramJson["Value"][3]
                };
            } else if (param.type == SHADER_UNIFORM_VEC3) {
                param.value = Vector3{paramJson["Value"][0], paramJson["Value"][1], paramJson["Value"][2]};
            } else if (param.type == SHADER_UNIFORM_VEC2) {
                param.value = Vector2{paramJson["Value"][0], paramJson["Value"][1]};
            } else if (param.type == SHADER_UNIFORM_FLOAT) {
                param.value = paramJson.at("Value").get<float>();
            } else if (param.type == SHADER_UNIFORM_INT) {
                param.value = paramJson.at("Value").get<int>();
            }
            shaderParams.emplace_back(param);
        }

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
        Vector2 *texcoords = static_cast<Vector2 *>(RL_MALLOC(verticesCount*sizeof(Vector2)));
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
                // uv (0.25, 0.25) -> (0.5, 0.5)
                texcoords[x * size[2] + z + verticesAdded] = {
                    x * 0.25f / (size[0] - 1) + 0.25f,
                    z * 0.25f / (size[2] - 1) + 0.25f
                };

                // Top
                vertices[x * size[2] + z + verticesPerPlane + verticesAdded] = {
                    x * sizeWorld.x / (size[0] - 1),
                    sizeWorld.y,
                    z * sizeWorld.z / (size[2] - 1)
                };
                normals[x * size[2] + z + verticesPerPlane + verticesAdded] = {0, 1, 0};
                // uv (0.25, 0.75) -> (0.5, 1.0)
                texcoords[x * size[2] + z + verticesPerPlane + verticesAdded] = {
                    (1 - static_cast<float>(x) / (size[0] - 1)) * 0.25f + 0.25f,
                    static_cast<float>(z) / (size[2] - 1) * 0.25f + 0.75f
                };
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
                // uv (0.0, 0.5) -> (0.25, 0.75)
                texcoords[y * size[2] + z + verticesAdded] = {
                    (1 - static_cast<float>(y) / (size[1] - 1)) * 0.25f,
                    z * 0.25f / (size[2] - 1) + 0.5f
                };

                // Right
                vertices[y * size[2] + z + verticesPerPlane + verticesAdded] = {
                    sizeWorld.x,
                    y * sizeWorld.y / (size[1] - 1),
                    z * sizeWorld.z / (size[2] - 1)
                };
                normals[y * size[2] + z + verticesPerPlane + verticesAdded] = {1, 0, 0};
                // uv (0.5, 0.5) -> (0.75, 0.75)
                texcoords[y * size[2] + z + verticesPerPlane + verticesAdded] = {
                    y * 0.25f / (size[1] - 1) + 0.5f,
                    z * 0.25f / (size[2] - 1) + 0.5f
                };
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
                // Back
                vertices[x * size[1] + y + verticesAdded] = {
                    x * sizeWorld.x / (size[0] - 1),
                    y * sizeWorld.y / (size[1] - 1),
                    0,
                };
                normals[x * size[1] + y + verticesAdded] = {0, 0, -1};
                // uv (0.75, 0.5) -> (1.0, 0.75)
                texcoords[x * size[1] + y + verticesAdded] = {
                    (1 - static_cast<float>(x) / (size[0] - 1)) * 0.25f + 0.75f,
                    y * 0.25f / (size[1] - 1) + 0.5f
                };

                // Front
                vertices[x * size[1] + y + verticesPerPlane + verticesAdded] = {
                    x * sizeWorld.x / (size[0] - 1),
                    y * sizeWorld.y / (size[1] - 1),
                    sizeWorld.z,
                };
                normals[x * size[1] + y + verticesPerPlane + verticesAdded] = {0, 0, 1};
                // uv (0.25, 0.5) -> (0.5, 0.75)
                texcoords[x * size[1] + y + verticesPerPlane + verticesAdded] = {
                    x * 0.25f / (size[0] - 1) + 0.25f,
                    y * 0.25f / (size[1] - 1) + 0.5f
                };
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
        for (int i = 0; i < mesh.vertexCount; i++) {
            mesh.texcoords[2 * i] = texcoords[i].x;
            mesh.texcoords[2 * i + 1] = 1 - texcoords[i].y;
        }

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
        RL_FREE(texcoords);
        RL_FREE(indices);

        UploadMesh(&mesh, false);

        return mesh;
    }

    // Draw a model (with texture if set)
    static void DrawModelWithShader(Model model, Vector3 position, float scale, Color tint, Shader shader) {
        const Vector3 vScale = {scale, scale, scale};
        const Vector3 rotationAxis = {0.0f, 1.0f, 0.0f};

        DrawModelWithShaderEx(model, position, rotationAxis, 0.0f, vScale, tint, shader);
    }

    // Draw a model with extended parameters
    static void DrawModelWithShaderEx(Model model, Vector3 position, Vector3 rotationAxis, float rotationAngle,
                                      Vector3 scale, Color tint, Shader shader) {
        // Calculate transformation matrix from function parameters
        // Get transform matrix (rotation -> scale -> translation)
        Matrix matScale = MatrixScale(scale.x, scale.y, scale.z);
        Matrix matRotation = MatrixRotate(rotationAxis, rotationAngle * DEG2RAD);
        Matrix matTranslation = MatrixTranslate(position.x, position.y, position.z);

        Matrix matTransform = MatrixMultiply(MatrixMultiply(matScale, matRotation), matTranslation);

        // Combine model transformation matrix (model.transform) with matrix generated by function parameters (matTransform)
        model.transform = MatrixMultiply(model.transform, matTransform);

        for (int i = 0; i < model.meshCount; i++) {
            Color color = model.materials[model.meshMaterial[i]].maps[MATERIAL_MAP_DIFFUSE].color;

            Color colorTint = WHITE;
            colorTint.r = (unsigned char) (((int) color.r * (int) tint.r) / 255);
            colorTint.g = (unsigned char) (((int) color.g * (int) tint.g) / 255);
            colorTint.b = (unsigned char) (((int) color.b * (int) tint.b) / 255);
            colorTint.a = (unsigned char) (((int) color.a * (int) tint.a) / 255);

            model.materials[model.meshMaterial[i]].maps[MATERIAL_MAP_DIFFUSE].color = colorTint;
            Shader originalShader = model.materials[model.meshMaterial[i]].shader;
            model.materials[model.meshMaterial[i]].shader = shader;

            DrawMesh(model.meshes[i], model.materials[model.meshMaterial[i]], model.transform);

            model.materials[model.meshMaterial[i]].shader = originalShader;
            model.materials[model.meshMaterial[i]].maps[MATERIAL_MAP_DIFFUSE].color = color;
        }
    }

    static Shader GetUVShader() {
        static Shader shader = {0}; // Ensure uninitialized state
        if (shader.id == 0) {
            // Load only if shader is not initialized
            static std::string vsPath = INTERNAL_ASSET_DIR + std::string("/shaders/default.vert");
            static std::string fsPath = INTERNAL_ASSET_DIR + std::string("/shaders/uv.frag");
            shader = LoadShader(vsPath.c_str(), fsPath.c_str());
        }
        return shader;
    }

    static Shader GetNormalShader() {
        static Shader shader = {0}; // Ensure uninitialized state
        if (shader.id == 0) {
            // Load only if shader is not initialized
            static std::string vsPath = INTERNAL_ASSET_DIR + std::string("/shaders/default.vert");
            static std::string fsPath = INTERNAL_ASSET_DIR + std::string("/shaders/normal.frag");
            shader = LoadShader(vsPath.c_str(), fsPath.c_str());
        }
        return shader;
    }

    static TextureWrap StringToTextureWrapEnum(std::string s) {
        //     TEXTURE_WRAP_REPEAT = 0,                // Repeats texture in tiled mode
        //     TEXTURE_WRAP_CLAMP,                     // Clamps texture to edge pixel in tiled mode
        //     TEXTURE_WRAP_MIRROR_REPEAT,             // Mirrors and repeats the texture in tiled mode
        //     TEXTURE_WRAP_MIRROR_CLAMP               // Mirrors and clamps to border the texture in tiled mode
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        if (s == "REPEAT") return TEXTURE_WRAP_REPEAT;
        if (s == "CLAMP") return TEXTURE_WRAP_CLAMP;
        if (s == "MIRROR REPEAT") return TEXTURE_WRAP_MIRROR_REPEAT;
        if (s == "MIRROR CLAMP") return TEXTURE_WRAP_MIRROR_CLAMP;
    }

    static TextureFilter StringToTextureFilterEnum(std::string s) {
        //     TEXTURE_FILTER_POINT = 0,               // No filter, just pixel approximation
        //     TEXTURE_FILTER_BILINEAR,                // Linear filtering
        //     TEXTURE_FILTER_TRILINEAR,               // Trilinear filtering (linear with mipmaps)
        //     TEXTURE_FILTER_ANISOTROPIC_4X,          // Anisotropic filtering 4x
        //     TEXTURE_FILTER_ANISOTROPIC_8X,          // Anisotropic filtering 8x
        //     TEXTURE_FILTER_ANISOTROPIC_16X,         // Anisotropic filtering 16x
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        if (s == "POINT") return TEXTURE_FILTER_POINT;
        if (s == "BILINEAR") return TEXTURE_FILTER_BILINEAR;
        if (s == "TRILINEAR") return TEXTURE_FILTER_TRILINEAR;
        if (s == "ANISOTROPIC 4X") return TEXTURE_FILTER_ANISOTROPIC_4X;
        if (s == "ANISOTROPIC 8X") return TEXTURE_FILTER_ANISOTROPIC_8X;
        if (s == "ANISOTROPIC 16X") return TEXTURE_FILTER_ANISOTROPIC_16X;
    }

    static ShaderUniformDataType StringToShaderUniformDataType(std::string s) {
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        if (s == "FLOAT") return SHADER_UNIFORM_FLOAT;
        if (s == "VEC2") return SHADER_UNIFORM_VEC2;
        if (s == "VEC3") return SHADER_UNIFORM_VEC3;
        if (s == "VEC4") return SHADER_UNIFORM_VEC4;
        if (s == "INT") return SHADER_UNIFORM_INT;
        // TODO: add more
    }

    static Model GetArrowModel() {
        static Model model = {0}; // Ensure uninitialized state
        if (model.meshCount == 0) {
            // Load only if model is not initialized
            const Mesh arrowHead = GenMeshCone(0.1f, 0.4f, 16);
            for (int i = 0; i < arrowHead.vertexCount; i++) {
                arrowHead.vertices[i * 3 + 1] += 0.6f;
            }
            UpdateMeshBuffer(arrowHead, 0, arrowHead.vertices, arrowHead.vertexCount * 3 * sizeof(float), 0);
            const Mesh arrowTail = GenMeshCylinder(0.04f, 0.6f, 16);
            model.meshCount = 2;
            model.meshes = static_cast<Mesh *>(RL_REALLOC(model.meshes, 2*sizeof(Mesh)));
            model.meshes[0] = arrowTail;
            model.meshes[1] = arrowHead;

            model.materialCount = 1;
            model.materials = (Material *) RL_CALLOC(model.materialCount, sizeof(Material));
            model.materials[0] = LoadMaterialDefault();

            model.meshMaterial = (int *) RL_CALLOC(model.meshCount, sizeof(int));
            model.meshMaterial[0] = 0; // First material index
            model.meshMaterial[1] = 0; // Second material index

            model.transform = MatrixIdentity();
        }
        return model;
    }

    static void RecalculateMeshNormals(Mesh *mesh) {
        const float *vertices = mesh->vertices;
        float *normals = mesh->normals;
        const unsigned short *indices = mesh->indices;

        // For each triangle
        for (int i = 0; i < mesh->triangleCount * 3; i += 3) {
            // Get triangle vertices
            const Vector3 v0 = {
                vertices[indices[i] * 3],
                vertices[indices[i] * 3 + 1],
                vertices[indices[i] * 3 + 2]
            };
            const Vector3 v1 = {
                vertices[indices[i + 1] * 3],
                vertices[indices[i + 1] * 3 + 1],
                vertices[indices[i + 1] * 3 + 2]
            };
            const Vector3 v2 = {
                vertices[indices[i + 2] * 3],
                vertices[indices[i + 2] * 3 + 1],
                vertices[indices[i + 2] * 3 + 2]
            };

            // Calculate triangle normal
            const Vector3 edge1 = Vector3Subtract(v1, v0);
            const Vector3 edge2 = Vector3Subtract(v2, v0);
            const Vector3 normal = Vector3Normalize(Vector3CrossProduct(edge1, edge2));

            // Add to vertex normals
            for (int j = 0; j < 3; j++) {
                const int idx = indices[i + j] * 3;
                normals[idx] = normal.x;
                normals[idx + 1] = normal.y;
                normals[idx + 2] = normal.z;
            }
        }

        // Update GPU buffer
        UpdateMeshBuffer(*mesh, 2, normals, mesh->vertexCount * 3 * sizeof(float), 0);
    }

    static Vector2 SampleBezier(Vector2 A, Vector2 B, Vector2 C, Vector2 D, float t) {
        const Vector2 AB = Vector2Lerp(A, B, t);
        const Vector2 BC = Vector2Lerp(B, C, t);
        const Vector2 CD = Vector2Lerp(C, D, t);
        const Vector2 ABC = Vector2Lerp(AB, BC, t);
        const Vector2 BCD = Vector2Lerp(BC, CD, t);
        return Vector2Lerp(ABC, BCD, t);
    }

    static void DrawLineBezier(Vector2 A, Vector2 B, Vector2 C, Vector2 D, float thick, Color color, Color color2) {
        Vector2 previous = A;
        Vector2 current = {0};

        Vector2 points[2 * SPLINE_SEGMENT_DIVISIONS + 2] = {0};

        for (int i = 1; i <= SPLINE_SEGMENT_DIVISIONS; i++) {
            const float t = (float) i / (float) SPLINE_SEGMENT_DIVISIONS;
            current = SampleBezier(A, B, C, D, t);

            const float dy = current.y - previous.y;
            const float dx = current.x - previous.x;
            const float size = 0.5f * thick / sqrtf(dx * dx + dy * dy);

            if (i == 1) {
                points[0].x = previous.x + dy * size;
                points[0].y = previous.y - dx * size;
                points[1].x = previous.x - dy * size;
                points[1].y = previous.y + dx * size;
            }

            points[2 * i + 1].x = current.x - dy * size;
            points[2 * i + 1].y = current.y + dx * size;
            points[2 * i].x = current.x + dy * size;
            points[2 * i].y = current.y - dx * size;

            previous = current;
        }

        // DrawTriangleStrip(points, 2 * SPLINE_SEGMENT_DIVISIONS + 2, color);
        int pointCount = 2 * SPLINE_SEGMENT_DIVISIONS + 2;
        if (pointCount >= 3) {
            rlBegin(RL_TRIANGLES);

            for (int i = 2; i < pointCount; i++) {
                float t = (float) i / (float) pointCount;
                Color colorBlend = ColorLerp(color, color2, t);
                rlColor4ub(colorBlend.r, colorBlend.g, colorBlend.b, colorBlend.a);
                if ((i % 2) == 0) {
                    rlVertex2f(points[i].x, points[i].y);
                    rlVertex2f(points[i - 2].x, points[i - 2].y);
                    rlVertex2f(points[i - 1].x, points[i - 1].y);
                } else {
                    rlVertex2f(points[i].x, points[i].y);
                    rlVertex2f(points[i - 1].x, points[i - 1].y);
                    rlVertex2f(points[i - 2].x, points[i - 2].y);
                }
            }
            rlEnd();
        }
    }

    // Draw a color-filled rectangle with pro parameters
    static void DrawRectangleUV(Rectangle rec, Vector2 origin, float rotation, Color color) {
        Vector2 topLeft = {0};
        Vector2 topRight = {0};
        Vector2 bottomLeft = {0};
        Vector2 bottomRight = {0};

        // Only calculate rotation if needed
        if (rotation == 0.0f) {
            float x = rec.x - origin.x;
            float y = rec.y - origin.y;
            topLeft = (Vector2){x, y};
            topRight = (Vector2){x + rec.width, y};
            bottomLeft = (Vector2){x, y + rec.height};
            bottomRight = (Vector2){x + rec.width, y + rec.height};
        } else {
            const float sinRotation = sinf(rotation * DEG2RAD);
            const float cosRotation = cosf(rotation * DEG2RAD);
            float x = rec.x;
            float y = rec.y;
            const float dx = -origin.x;
            const float dy = -origin.y;

            topLeft.x = x + dx * cosRotation - dy * sinRotation;
            topLeft.y = y + dx * sinRotation + dy * cosRotation;

            topRight.x = x + (dx + rec.width) * cosRotation - dy * sinRotation;
            topRight.y = y + (dx + rec.width) * sinRotation + dy * cosRotation;

            bottomLeft.x = x + dx * cosRotation - (dy + rec.height) * sinRotation;
            bottomLeft.y = y + dx * sinRotation + (dy + rec.height) * cosRotation;

            bottomRight.x = x + (dx + rec.width) * cosRotation - (dy + rec.height) * sinRotation;
            bottomRight.y = y + (dx + rec.width) * sinRotation + (dy + rec.height) * cosRotation;
        }

#if defined(SUPPORT_QUADS_DRAW_MODE)
        rlSetTexture(GetShapesTexture().id);
        Rectangle shapeRect = GetShapesTextureRectangle();

        rlBegin(RL_QUADS);

        rlNormal3f(0.0f, 0.0f, 1.0f);
        rlColor4ub(color.r, color.g, color.b, color.a);

        rlTexCoord2f(0.0f, 0.0f);
        rlVertex2f(topLeft.x, topLeft.y);

        rlTexCoord2f(0.0f, 1.0f);
        rlVertex2f(bottomLeft.x, bottomLeft.y);

        rlTexCoord2f(1.0f, 1.0f);
        rlVertex2f(bottomRight.x, bottomRight.y);

        rlTexCoord2f(1.0f, 0.0f);
        rlVertex2f(topRight.x, topRight.y);

        rlEnd();

        rlSetTexture(0);
#else
        rlBegin(RL_TRIANGLES);

        rlColor4ub(color.r, color.g, color.b, color.a);

        rlVertex2f(topLeft.x, topLeft.y);
        rlVertex2f(bottomLeft.x, bottomLeft.y);
        rlVertex2f(topRight.x, topRight.y);

        rlVertex2f(topRight.x, topRight.y);
        rlVertex2f(bottomLeft.x, bottomLeft.y);
        rlVertex2f(bottomRight.x, bottomRight.y);

        rlEnd();
#endif
    }

    static bool CheckCollitionPointBezier(Vector2 check, float radius, Vector2 A, Vector2 B, Vector2 C, Vector2 D,
                                          int samples = 10) {
        float rsquare = radius * radius;
        for (int i = 0; i < samples; i++) {
            const float t = (float) i / (float) samples;
            const Vector2 point = SampleBezier(A, B, C, D, t);
            if (Vector2DistanceSqr(check, point) < rsquare) {
                return true;
            }
        }
        return false;
    }

    static Rectangle GetRectFromPoints(Vector2 A, Vector2 B) {
        Vector2 size = {
            fabsf(A.x - B.x),
            fabsf(A.y - B.y)
        };
        Vector2 position = {
            fminf(A.x, B.x),
            fminf(A.y, B.y)
        };
        return Rectangle{position.x, position.y, size.x, size.y};
    }

    static RenderTexture2D LoadRenderTextureX(int width, int height, int format = PIXELFORMAT_UNCOMPRESSED_R8G8B8) {
        RenderTexture2D target = {0};

        target.id = rlLoadFramebuffer(); // Load an empty framebuffer

        if (target.id > 0) {
            rlEnableFramebuffer(target.id);

            // Create color texture (default to RGBA)
            target.texture.id = rlLoadTexture(NULL, width, height, format, 1);
            target.texture.width = width;
            target.texture.height = height;
            target.texture.format = format;
            target.texture.mipmaps = 1;

            // Create depth renderbuffer/texture
            target.depth.id = rlLoadTextureDepth(width, height, true);
            target.depth.width = width;
            target.depth.height = height;
            target.depth.format = 19; //DEPTH_COMPONENT_24BIT?
            target.depth.mipmaps = 1;

            // Attach color texture and depth renderbuffer/texture to FBO
            rlFramebufferAttach(target.id, target.texture.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
            rlFramebufferAttach(target.id, target.depth.id, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);

            // Check if fbo is complete with attachments (valid)
            if (rlFramebufferComplete(target.id))
                TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", target.id);

            rlDisableFramebuffer();
        } else
            TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");

        return target;
    }
};

#endif //RAYLIBUTILS_H
