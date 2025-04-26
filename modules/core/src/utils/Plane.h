#ifndef PLANE_H
#define PLANE_H

#include <raylib.h>
#include <raymath.h>

struct Plane {
    Vector3 normal;
    Vector3 p;

    Plane() = default;

    Plane(const Vector3 &normal, const Vector3 &p) : normal(Vector3Normalize(normal)), p(p) {
    }

    Plane(const Vector3 &a, const Vector3 &b, const Vector3 &c) {
        const Vector3 ab = b - a;
        const Vector3 bc = c - b;
        normal = Vector3Normalize(Vector3CrossProduct(ab, bc));
        p = b;
    }

    double distanceWithSign(const Vector3 &other) const {
        return Vector3DotProduct(other - p, normal);
    }

    Vector3 closestPoint(const Vector3 &other) const {
        return other - normal * distanceWithSign(other);
    }
};

#endif //PLANE_H
