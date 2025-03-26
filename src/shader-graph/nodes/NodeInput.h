//
// Created by Qiaozhi Lei on 3/24/25.
//

#ifndef NODEINPUT_H
#define NODEINPUT_H
#include <string>

#include "raylib.h"

#include "../Context.h"

class NodeOutput;
class Node;

class NodeInput {
public:
    std::string name;
    NodeOutput *source = nullptr;
    Vector2 circleCenter;
    float radius = 0;
    bool hovering;

    Color GetColor() const;

    void Draw(Rectangle &rect);

    void Update(Context &context);

    void Resolve(Context & context);
};


#endif //NODEINPUT_H
