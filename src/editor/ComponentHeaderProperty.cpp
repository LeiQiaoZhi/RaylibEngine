#include <raygui.h>
#include "ComponentHeaderProperty.h"
#include "Editor.h"
#include "../utils/RaylibUtils.h"
#include "../GCM/GameObject.h"

void ComponentHeaderProperty::OnEditorGUI(Rectangle &rect) {
    Color color = GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_NORMAL));
    if (!*enabled) {
        color = GetColor(GuiGetStyle(DEFAULT, BASE_COLOR_DISABLED));
    }

    // background
    DrawRectangleRec({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f}, color);

    const float checkboxWidth = Editor::TextSize() * 1.0f;
    const float removeWidth = Editor::TextWidth("Remove") + Editor::LargeGap();
    const char *title = GuiIconText(folded ? ICON_ARROW_RIGHT_FILL : ICON_ARROW_DOWN_FILL, label.c_str());
    if (GuiLabelButton({
                           rect.x, rect.y, rect.width - checkboxWidth - removeWidth, Editor::TextSize() * 1.0f
                       }, title)) {
        folded = !folded;
    }

    // Enable/Disable
    Rectangle checkboxRect = {rect.x + rect.width - checkboxWidth, rect.y, checkboxWidth, Editor::TextSize() * 1.0f};
    GuiCheckBox(checkboxRect, nullptr, enabled);

    // Remove
    Rectangle removeRect = {checkboxRect.x - removeWidth, rect.y, removeWidth, Editor::TextSize() * 1.0f};
    if (GuiButton(removeRect, "Remove")) {
        showMessageBox = true;
    }
    if (showMessageBox) {
        Rectangle messageBoxRect = {
            rect.x, rect.y + Editor::TextSize() + Editor::SmallGap(),
            rect.width, Editor::TextSize() * 4.0f + Editor::SmallGap()
        };
        int result = GuiMessageBox(
            messageBoxRect,
            GuiIconText(ICON_FILE_DELETE, "Remove Component?"), nullptr, "Yes;No");
        if (result >= 0) // 0: Close, 1: Yes, 2: No
            showMessageBox = false;
        if (result == 1) {
            component->gameObject->TryRemoveComponent(component, &statusText, &statusWarning);
        }

        rect.y += messageBoxRect.height + Editor::SmallGap();
    }
    rect.y += Editor::TextSize() + Editor::SmallGap();

    Editor::DrawStatusInfoBox(rect, statusText, statusWarning);
}

float ComponentHeaderProperty::GetEditorHeight() const {
    return Editor::TextSize() * 1 + Editor::SmallGap();
}

bool ComponentHeaderProperty::IsFolded() const {
    if (component == nullptr)
        return true;
    return folded || component->removed;
}

nlohmann::json ComponentHeaderProperty::ToJson() const {
    nlohmann::json j;
    j["folded"] = folded;
    return j;
}

void ComponentHeaderProperty::FromJson(const nlohmann::json &json) {
    folded = json["folded"];
}
