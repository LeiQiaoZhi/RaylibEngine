//
// Created by Qiaozhi Lei on 3/24/25.
//

#ifndef NodeIO_H
#define NodeIO_H
#include <string>

#include "raylib.h"

#include "../Context.h"

class Node;

class NodeIO {
protected:
    std::string name;
    Vector2 circleCenter;
    float radius;
    bool hovering;

    virtual Color GetColor() = 0;
    virtual void Draw(Rectangle &rect) = 0;
    virtual void Update(Context &context) = 0;
};


#endif //NodeIO_H
