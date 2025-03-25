#ifndef SPHERE_H
#define SPHERE_H

#include <raylib.h>
#include <raymath.h>

struct Sphere {
    Vector3 center;
    float radius;

    Sphere() = default;

    Sphere(const Vector3 &center, float radius) : center(center), radius(radius) {
    }

    double distanceWithSign(const Vector3 &other) const {
        return Vector3Distance(center, other) - radius;
    }

    Vector3 closestPoint(const Vector3 &other) const {
        const Vector3 toCenter = Vector3Normalize(center - other);
        return other + toCenter * distanceWithSign(other);
    }
};

#endif //SPHERE_H
