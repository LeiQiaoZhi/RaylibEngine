//
// Created by Qiaozhi Lei on 3/24/25.
//

#ifndef NodeOutput_H
#define NodeOutput_H
#include <string>

#include "NodeIO.h"
#include "raylib.h"

#include "../Context.h"

enum class ShaderType;
class NodeInput;
class Node;

class NodeOutput : public NodeIO {
public:
    using NodeIO::NodeIO;

    std::list<NodeInput *> targets = {};

    Color GetColor() const override;

    void Draw(Rectangle &rect, Context &context) override;

    void Update(Context &context) override;

    float GetWidth() const;

    void Resolve(Context &context);

    std::string GetVarName() const;
};


#endif //NodeOutput_H
