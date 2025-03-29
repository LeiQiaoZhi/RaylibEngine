//
// Created by Qiaozhi Lei on 3/24/25.
//

#ifndef NodeIO_H
#define NodeIO_H
#include <string>
#include "../ShaderTypes.h"

#include "raylib.h"

#include "../Context.h"

class Node;

class NodeIO {
public:
    std::string name;
    ShaderType type;
    Node *parentNode = nullptr;

    Vector2 circleCenter;
    float radius;
    bool hovering;
    bool dragging;

    explicit NodeIO(std::string name, ShaderType type, Node *parentNode)
        : name(std::move(name)), type(type), parentNode(parentNode) {
    }

    virtual Color GetColor() const = 0;

    virtual void Draw(Rectangle &rect, Context &context) = 0;

    virtual void Update(Context &context) = 0;
};


#endif //NodeIO_H
