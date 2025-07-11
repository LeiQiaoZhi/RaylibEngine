//
// Created by Qiaozhi Lei on 3/24/25.
//

#ifndef NODE_H
#define NODE_H
#include <vector>

#include "NodeInput.h"
#include "NodeOutput.h"
#include "raylib.h"
#include <set>

#include "../Context.h"
#include "editor/FoldoutProperty.h"
#include "utils/Utils.h"

class Node {
public:
    Node() {
        uid = Utils::GenerateUID(name);
    }

    explicit Node(nlohmann::json j);

    explicit Node(nlohmann::json j, bool dummyFromSave);

    explicit Node(Node *node);

    void LoadConnections(const nlohmann::json &j, Context *context);

    virtual nlohmann::json ToJson() const;

    ~Node() = default;

    virtual void OnDraw(Context &context);

    virtual void Update(Context &context);

    virtual void Resolve(Context &context);

    virtual void OnEditorGUI(Rectangle &rect, Context &context);

    void AddInput(const std::string &name, ShaderType type) {
        inputs.emplace_back(name, type, this);
    }

    void AddOutput(const std::string &name, ShaderType type) {
        outputs.emplace_back(name, type, this);
    }

    void AddInput(const std::string &name, ShaderType type, uint uid) {
        inputs.emplace_back(name, type, this, uid);
    }

    void AddOutput(const std::string &name, ShaderType type, uint uid) {
        outputs.emplace_back(name, type, this, uid);
    }

    bool operator==(const Node &rhs) const {
        return uid == rhs.uid;
    }

    NodeOutput *FindOutputByUID(uint uid) {
        for (auto &output: outputs) {
            if (output.uid == uid) {
                return &output;
            }
        }
        return nullptr;
    }

    NodeInput *FindInputByUID(uint uid) {
        for (auto &input: inputs) {
            if (input.uid == uid) {
                return &input;
            }
        }
        return nullptr;
    }

    std::set<Node *> GetNeighboursFromInputs() const;

    std::set<Node *> GetNeighboursFromOutputs() const;

    std::set<Node *> GetNeighbours() const {
        std::set<Node *> neighboursIn = GetNeighboursFromInputs();
        std::set<Node *> neighboursOut = GetNeighboursFromOutputs();
        neighboursIn.insert(neighboursOut.begin(), neighboursOut.end());
        return neighboursIn;
    }


    std::string name = "Node";
    Vector2 position = {0, 0};
    Vector2 size = {100, 100};
    uint uid;

    std::string glsl;
    std::string hlsl;

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

    // preview
    Shader shader = LoadShader(0, INTERNAL_ASSET_DIR "/shaders/error.frag");
    std::string previewCode;
    bool showPreview;
    int previewOutputIndex = 0;
    bool statusError = false;
};


#endif //NODE_H
