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
#include "../../common/editor/SingleValueProperty.h"

class NodeOutput;
class Node;

class NodeInput : public NodeIO {
public:
    using NodeIO::NodeIO;

    NodeOutput *source = nullptr;

    bool nameEditMode;
    DropdownProperty typeDropdown = DropdownProperty(GetShaderTypeNames(), 0);
    float floatValue = 0;
    char floatBuffer[256] = "0"; // TODO: initialize with value
    bool floatEditMode = false;
    // FloatProperty floatProperty = FloatProperty(&floatValue, name);

    Color GetColor() const override;

    void Draw(Rectangle &rect, Context &context) override;

    void Update(Context &context) override;

    void Resolve(Context &context);

    void OnEditorGUI(Rectangle &rect);

    nlohmann::json ToJson() const override;

    std::string GetVarName() const;
};


#endif //NODEINPUT_H
