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
};

#endif //RAYLIBUTILS_H
