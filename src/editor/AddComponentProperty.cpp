#include <raygui.h>
#include "AddComponentProperty.h"
#include "Editor.h"
#include "../utils/RaylibUtils.h"
#include "../utils/Utils.h"

void AddComponentProperty::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;

    // status
    Editor::DrawStatusInfoBox(rect, statusText, statusWarning);

    const float buttonWidth = Editor::TextWidth("Add Component") + Editor::LargeGap();
    Rectangle dropRect = {rect.x, rect.y, rect.width - buttonWidth - Editor::SmallGap(), Editor::TextSize() * 1.5f};

    dropdownProperty.OnEditorGUI(dropRect);

    if (GuiButton(Rectangle{rect.x + rect.width - buttonWidth, rect.y, buttonWidth, Editor::TextSize() * 1.5f},
                  "Add Component")) {
        if (gameObject == nullptr) {
            statusText = "GameObject is null";
            statusWarning = true;
        } else {
            const std::string componentType = dropdownProperty.GetSelectedOption();
            const std::function<Component *()> componentFactory = Component::ComponentTypeMap()[componentType];
            Component *component = componentFactory();
            gameObject->TryAddComponent(component, &statusText, &statusWarning);
        }
    }
    rect.y += Editor::TextSize() * 1.5f + Editor::SmallGap();


    height = rect.y - originalY;
}

float AddComponentProperty::GetEditorHeight() const {
    return std::max(height, dropdownProperty.GetEditorHeight());
}

nlohmann::json AddComponentProperty::ToJson() const {
    nlohmann::json j;
    return j;
}

void AddComponentProperty::FromJson(const nlohmann::json &json) {
}
