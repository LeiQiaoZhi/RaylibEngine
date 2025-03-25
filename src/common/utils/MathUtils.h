#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <vector>
#include <raylib.h>

using Matrix3D = std::vector<std::vector<std::vector<Vector3> > >;

class MathUtils {
public:
    static Matrix3D CreateMatrix3D(int x, int y, int z) {
        return Matrix3D(x, std::vector(y, std::vector<Vector3>(z)));
    }
};

#endif //MATHUTILS_H
