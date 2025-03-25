//
// Created by Qiaozhi Lei on 3/24/25.
//

#ifndef NODE_H
#define NODE_H
#include "raylib.h"

#include "../Context.h"

class Node {
public:
    Node() = default;

    ~Node() = default;

    void OnDraw();

    void Update(Context context);

private:
    // type, inputs, outputs, size
    Vector2 position;
    Vector2 size = {100, 100};
};


#endif //NODE_H
