#ifndef SCRIPTCOMPONENT_H
#define SCRIPTCOMPONENT_H
#include <string>

#include "Component.h"
#include "raylib.h"
#include "../../editor/FoldoutProperty.h"
#include "../../editor/VectorProperty.h"
#include <sol/sol.hpp>

#include "../../editor/SingleValueProperty.h"


class LuaProperty;

class ScriptComponent final : public Component {
public:
    explicit ScriptComponent();

    void OnEditorGUI(Rectangle &rect) override;

    void OnEditorDraw(Scene *scene) const override;

    void OnDrawGizmos(Scene *scene) const override;

    void OnDrawGizmosSelected(Scene *scene) const override;

    void OnDrawGizmosBottom(Scene *scene) const override;

    float GetEditorHeight() const override;

    void EditorStart() override;

    void EditorUpdate() override;

    void Update() override;

    nlohmann::json ToJson() const override;

    void FromJson(const nlohmann::json &json) override;

    void LoadScriptFromPath(const std::string &path);

private:
    char nameBuffer[256];
    std::string loadedScriptName;
    bool nameEditMode = false;
    sol::environment luaEnv;
    sol::function updateFunc;
    sol::function editorUpdateFunc;

    // ui
    FoldoutProperty propertiesFoldout = FoldoutProperty("Properties");
    std::vector<LuaProperty> properties;
    bool statusWarning = false;
    std::string statusText;
    float height;
};


#endif //SCRIPTCOMPONENT_H
