//
// Created by Qiaozhi Lei on 3/24/25.
//

#ifndef NODE_H
#define NODE_H
#include <vector>

#include "NodeInput.h"
#include "NodeOutput.h"
#include "raylib.h"

#include "../Context.h"

class Node {
public:
    Node() = default;

    ~Node() = default;

    void OnDraw();

    void Update(Context &context);

    void Resolve(Context & context);

    // type, inputs, outputs, size
    Vector2 position;
    Vector2 size = {100, 100};

    std::vector<NodeInput> inputs;
    std::vector<NodeOutput> outputs;
    bool hovering;
    bool dragging;
};


#endif //NODE_H
