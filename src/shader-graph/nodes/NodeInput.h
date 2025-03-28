//
// Created by Qiaozhi Lei on 3/24/25.
//

#ifndef NODEINPUT_H
#define NODEINPUT_H
#include <set>
#include <string>

#include "raylib.h"

#include "NodeIO.h"
#include "../Context.h"
#include "../../common/editor/DropdownProperty.h"

class NodeOutput;
class Node;

class NodeInput : public NodeIO {
public:
    using NodeIO::NodeIO;

    NodeOutput *source = nullptr;

    bool nameEditMode;
    DropdownProperty typeDropdown = DropdownProperty(GetShaderTypeNames(), 0);


    Color GetColor() const override;

    void Draw(Rectangle &rect) override;

    void Update(Context &context) override;

    void Resolve(Context &context);

    void OnEditorGUI(Rectangle &rect);

    std::string GetVarName() const;
};


#endif //NODEINPUT_H
