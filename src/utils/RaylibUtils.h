#ifndef RAYLIBUTILS_H
#define RAYLIBUTILS_H

#include "raylib.h"
#include <iostream>

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
};

#endif //RAYLIBUTILS_H
