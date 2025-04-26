//
// Created by Qiaozhi Lei on 3/24/25.
//

#ifndef NodeIO_H
#define NodeIO_H
#include <string>
#include <nlohmann/json.hpp>

#include "../ShaderTypes.h"

#include "raylib.h"

#include "../Context.h"
#include "utils/Utils.h"

class NodeIO {
public:
    std::string name;
    ShaderType type;
    Node *parentNode = nullptr;
    uint uid = 0;

    Vector2 circleCenter;
    float radius;
    bool hovering;
    bool dragging;

    explicit NodeIO(std::string name, ShaderType type, Node *parentNode)
        : name(std::move(name)), type(type), parentNode(parentNode) {
        uid = Utils::GenerateUID(this->name);
    }

    explicit NodeIO(std::string name, ShaderType type, Node *parentNode, uint uid)
      : name(std::move(name)), type(type), parentNode(parentNode), uid(uid) {
    }

    virtual Color GetColor() const = 0;

    virtual void Draw(Rectangle &rect, Context &context) = 0;

    virtual void Update(Context &context) = 0;

    virtual nlohmann::json ToJson() const {
        nlohmann::json j;
        j["name"] = name;
        j["type"] = ShaderTypeToStringMap[type];
        j["uid"] = uid;
        return j;
    }
};


#endif //NodeIO_H
