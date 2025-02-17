#ifndef SHADERPARAM_H
#define SHADERPARAM_H

#include <variant>
#include <raylib.h>
#include <string>

using ShaderParamValue = std::variant<int, float, Vector2, Vector3, Vector4>;
struct ShaderParam {
    std::string name;
    ShaderUniformDataType type;
    ShaderParamValue value;
};

#endif //SHADERPARAM_H
