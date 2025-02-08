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
};

#endif //RAYLIBUTILS_H
