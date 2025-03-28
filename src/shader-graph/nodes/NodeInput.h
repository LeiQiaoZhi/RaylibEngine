//
// Created by Qiaozhi Lei on 3/24/25.
//

#ifndef NODEINPUT_H
#define NODEINPUT_H
#include <string>

#include "raylib.h"

#include "../Context.h"
#include "../../common/editor/DropdownProperty.h"
#include "../ShaderTypes.h"

class NodeOutput;
class Node;

class NodeInput {
public:
    std::string name;
    NodeOutput *source = nullptr;
    Vector2 circleCenter;
    float radius = 0;
    bool hovering;
    bool nameEditMode;
    DropdownProperty typeDropdown = DropdownProperty(GetShaderTypeNames(), 0);
    ShaderType type;

    Color GetColor() const;

    void Draw(Rectangle &rect);

    void Update(Context &context);

    void Resolve(Context &context);

    void OnEditorGUI(Rectangle &rect);
};


#endif //NODEINPUT_H
