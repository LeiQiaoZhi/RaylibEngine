#include "ScriptComponent.h"

#include <sstream>

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"
#include "../GameObject.h"
#include "../../editor/Editor.h"
#include "../../editor/LuaProperty.h"

ScriptComponent::ScriptComponent() {
    headerProperty.label = "Script";
}

void ScriptComponent::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;
    headerProperty.OnEditorGUI(rect);
    if (headerProperty.IsFolded()) {
        height = rect.y - originalY;
        return;
    }

    // Load script
    const float labelWidth = Editor::TextWidth("File path:") + Editor::SmallGap();
    GuiLabel({rect.x, rect.y, labelWidth, Editor::TextSize() * 1.0f}, "File path:");
    Editor::BeginIndent(rect, labelWidth);
    if (GuiTextBox(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f},
                   nameBuffer, 256, nameEditMode)) {
        nameEditMode = !nameEditMode;
    }
    Editor::EndIndent(rect, labelWidth);
    rect.y += Editor::TextSize() + Editor::SmallGap();

    if (GuiButton(Rectangle{rect.x, rect.y, rect.width, Editor::TextSize() * 1.5f}, "Load script")) {
        const std::string path = std::string(ASSET_DIR) + "/lua/" + nameBuffer;
        if (nameBuffer[0] == '\0') {
            statusText = "Filename is empty";
            statusWarning = true;
        } else if (!Utils::EndsWith(nameBuffer, ".lua")) {
            statusText = "Filename does not have .lua extension";
            statusWarning = true;
        } else if (!FileExists(path.c_str())) {
            statusText = "File does not exist";
            statusWarning = true;
        } else {
            LoadScriptFromPath(nameBuffer);
        }
    }
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();

    // Status
    Editor::DrawStatusInfoBox(rect, statusText, statusWarning);

    propertiesFoldout.OnEditorGUI(rect);
    if (!propertiesFoldout.IsFolded()) {
        Editor::BeginIndent(rect, Editor::LargeGap());
        // Properties
        for (auto &prop: properties) {
            prop.OnEditorGUI(rect);
        }
        Editor::EndIndent(rect, Editor::LargeGap());
    }

    height = rect.y - originalY;
}

float ScriptComponent::GetEditorHeight() const {
    return height;
}

void ScriptComponent::OnDrawGizmosBottom(Scene *scene) const {
}

void ScriptComponent::OnDraw(Scene *scene) const {
}

void ScriptComponent::Start() {
    if (!loadedScriptName.empty())
        LoadScriptFromPath(loadedScriptName);
}

void ScriptComponent::OnDrawGizmos(Scene *scene) const {
}

void ScriptComponent::OnDrawGizmosSelected(Scene *scene) const {
}

void ScriptComponent::Update() {
    if (updateFunc.valid())
        updateFunc();
}

nlohmann::json ScriptComponent::ToJson() const {
    nlohmann::json j;
    j["type"] = "ScriptComponent";
    if (!loadedScriptName.empty())
        j["loadedScriptName"] = loadedScriptName;
    return j;
}

void ScriptComponent::FromJson(const nlohmann::json &json) {
    if (json.contains("loadedScriptName")) {
        loadedScriptName = json["loadedScriptName"].get<std::string>();
        std::strncpy(nameBuffer, loadedScriptName.c_str(), sizeof(nameBuffer));
    }
}

void ScriptComponent::LoadScriptFromPath(const std::string &name) {
    const std::string path = std::string(ASSET_DIR) + "/lua/" + name;
    LuaManager &luaManager = gameObject->scene->runtimeContext.luaManager;
    sol::state &lua = luaManager.lua;
    luaEnv = sol::environment(lua, sol::create, lua.globals());

    auto result = lua.script_file(path, luaEnv);
    if (!result.valid()) {
        const sol::error err = result;
        statusText = err.what();
        statusWarning = true;
    } else {
        loadedScriptName = name;
        updateFunc = luaEnv["update"];
        luaManager.RegisterAtGameObjectLevel(luaEnv, gameObject);

        // Properties
        properties.clear();
        sol::optional<sol::table> propsTableO = luaEnv["properties"];
        if (!propsTableO) {
            statusText = "No properties table found";
            statusWarning = true;
            return;
        }
        sol::table propsTable = propsTableO.value();
        for (auto &pair: propsTable) {
            sol::object key = pair.first;
            sol::object value = pair.second;
            properties.emplace_back(key.as<std::string>(), value, propsTable);
        }

        statusText = "Script loaded";
        statusWarning = false;
    }
}
