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
#include "../../common/editor/FoldoutProperty.h"
#include "../../common/utils/Utils.h"

class Node {
public:
    Node() {
        uid = Utils::GenerateUID("Node"); // TODO: replace with actual type
    }

    explicit Node(nlohmann::json j);

    ~Node() = default;

    void OnDraw(Context &context);

    void Update(Context &context);

    void Resolve(Context &context);

    void OnEditorGUI(Rectangle &rect, Context &context);

    // TODO: type
    std::string name = "Node";
    Vector2 position = {0, 0};
    Vector2 size = {100, 100};
    int uid;
    std::string code;

    std::vector<NodeInput> inputs;
    std::vector<NodeOutput> outputs;

    // ui states
    bool hovering;
    bool dragging;

    // ui
    FoldoutProperty debugFoldout = FoldoutProperty("Debug", true);
    FoldoutProperty inputFoldout = FoldoutProperty("Inputs", true);
    FoldoutProperty outputFoldout = FoldoutProperty("Outputs", true);
    bool codeEditMode;
    float height;
};


#endif //NODE_H
