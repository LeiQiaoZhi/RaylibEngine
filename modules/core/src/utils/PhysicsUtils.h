#ifndef PHYSICSUTILS_H
#define PHYSICSUTILS_H

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <raylib.h>
#include <raymath.h>

class PhysicsUtils {
public:
    /// computes the force A receives
    static Vector3 Hooke(const Vector3 *a, const Vector3 *b, const double rest, const double k) {
        const Vector3 dir = {b->x - a->x, b->y - a->y, b->z - a->z};
        const double dir_length = Vector3Length(dir);
        const Vector3 dir_normalized = Vector3Normalize(dir);
        const double magnitude = k * (dir_length - rest);
        return dir_normalized * magnitude;
    }

    static Vector3 SpringDamp(const Vector3 *a, const Vector3 *b, const Vector3 relative_vel, const double k) {
        const Vector3 dir = {b->x - a->x, b->y - a->y, b->z - a->z};
        const Vector3 dir_normalized = Vector3Normalize(dir);
        const double projection = Vector3DotProduct(relative_vel, dir_normalized);

        return dir_normalized * -k * projection;
    }

    static void Euler(Matrix3D &positions, Matrix3D &velocities, const Matrix3D &accelerations, const double dt) {
        const int vertexCounts[3] = {
            static_cast<int>(positions.size()),
            static_cast<int>(positions[0].size()),
            static_cast<int>(positions[0][0].size())
        };

        for (int x = 0; x < vertexCounts[0]; x++) {
            for (int y = 0; y < vertexCounts[1]; y++) {
                for (int z = 0; z < vertexCounts[2]; z++) {
                    positions[x][y][z] += velocities[x][y][z] * dt;
                    velocities[x][y][z] += accelerations[x][y][z] * dt;
                }
            }
        }
    }
};

#endif //PHYSICSUTILS_H
