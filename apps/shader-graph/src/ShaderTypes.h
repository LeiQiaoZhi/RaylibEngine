//
// Created by Qiaozhi Lei on 3/27/25.
//

#ifndef SHADERTYPES_H
#define SHADERTYPES_H

#include <map>
#include <raylib.h>

enum class ShaderType {
    Float,
    Vec2,
    Vec3,
    Vec4,
    Mat2,
    Mat3,
    Mat4,
    Texture2D,
    Sampler2D,
    SamplerCube,
};

inline std::map<ShaderType, std::string> ShaderTypeToStringMap = {
    {ShaderType::Float, "float"},
    {ShaderType::Vec2, "vec2"},
    {ShaderType::Vec3, "vec3"},
    {ShaderType::Vec4, "vec4"},
    {ShaderType::Mat2, "mat2"},
    {ShaderType::Mat3, "mat3"},
    {ShaderType::Mat4, "mat4"},
    {ShaderType::Texture2D, "sampler2D"},
    {ShaderType::Sampler2D, "sampler2D"},
    {ShaderType::SamplerCube, "samplerCube"},
};

inline std::map<ShaderType, std::string> ShaderTypeToStringMapHLSL = {
    {ShaderType::Float, "float"},
    {ShaderType::Vec2, "float2"},
    {ShaderType::Vec3, "float3"},
    {ShaderType::Vec4, "float4"},
    {ShaderType::Mat2, "float2x2"},
    {ShaderType::Mat3, "float3x3"},
    {ShaderType::Mat4, "float4x4"},
    {ShaderType::Texture2D, "Texture2D"},
    {ShaderType::Sampler2D, "SamplerState"},
    {ShaderType::SamplerCube, "TextureCube"},
};

inline std::map<ShaderType, Color> ShaderTypeToColorMap = {
    {ShaderType::Float, {200, 200, 50, 255}},
    {ShaderType::Vec2, {100, 200, 100, 255}},
    {ShaderType::Vec3, {100, 150, 255, 255}},
    {ShaderType::Vec4, {255, 50, 255, 255}},
    {ShaderType::Mat2, {255, 128, 0, 255}},
    {ShaderType::Mat3, {128, 0, 255, 255}},
    {ShaderType::Mat4, {128, 128, 128, 255}},
    {ShaderType::Texture2D, {0, 250, 128, 255}},
    {ShaderType::Sampler2D, {0, 250, 128, 255}},
    {ShaderType::SamplerCube, {128, 128, 0, 255}},
};

inline std::map<std::string, ShaderType> ShaderTypeMap = {
    {"float", ShaderType::Float},
    {"vec2", ShaderType::Vec2},
    {"vec3", ShaderType::Vec3},
    {"vec4", ShaderType::Vec4},
    {"mat2", ShaderType::Mat2},
    {"mat3", ShaderType::Mat3},
    {"mat4", ShaderType::Mat4},
    {"sampler2D", ShaderType::Texture2D},
    {"samplerHLSL", ShaderType::Sampler2D},
    {"samplerCube", ShaderType::SamplerCube},
};

inline std::map<std::string, ShaderType> ShaderTypeMapHLSL = {
    {"float", ShaderType::Float},
    {"float2", ShaderType::Vec2},
    {"float3", ShaderType::Vec3},
    {"float4", ShaderType::Vec4},
    {"float2x2", ShaderType::Mat2},
    {"float3x3", ShaderType::Mat3},
    {"float4x4", ShaderType::Mat4},
    {"Texture2D", ShaderType::Texture2D},
    {"SamplerState", ShaderType::Sampler2D},
    {"TextureCube", ShaderType::SamplerCube},
};


inline std::vector<std::string> GetShaderTypeNames() {
    std::vector<std::string> names;
    for (const auto &pair: ShaderTypeMap) {
        names.push_back(pair.first);
    }
    return names;
}

#endif //SHADERTYPES_H
