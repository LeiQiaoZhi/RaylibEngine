#ifndef RAYLIBUTILS_H
#define RAYLIBUTILS_H

#include "raylib.h"
#include <iostream>
#include "raymath.h"

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

    static Vector3 GetScaleFromTransform(const Matrix& mat) {
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

    static void LoadMaterialFromFile(const char *path) {

    }
};

#endif //RAYLIBUTILS_H
