//
// Created by Qiaozhi Lei on 3/24/25.
//

#ifndef NodeOutput_H
#define NodeOutput_H
#include <string>

#include "raylib.h"

#include "../Context.h"

class NodeInput;
class Node;

class NodeOutput {
public:
    std::string name;
    NodeInput *target = nullptr;
    Vector2 circleCenter;
    float radius;
    bool hovering;
    bool dragging;

    Color GetColor() const;

    void Draw(Rectangle &rect);

    float GetWidth() const;

    void Update(Context &context);

    void Resolve(Context & context);
};


#endif //NodeOutput_H
