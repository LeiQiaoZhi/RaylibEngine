//
// Created by Qiaozhi Lei on 3/24/25.
//

#include "NodeInput.h"

#include "Node.h"
#include "NodeOutput.h"

#include "../../common/editor/Editor.h"
#include "../../common/utils/RaylibUtils.h"

Color NodeInput::GetColor() const {
    const float alpha = hovering ? 1.0f : 0.7f;
    if (source == nullptr) {
        return Fade(GRAY, alpha);
    }
    return Fade(GREEN, alpha);
}

void NodeInput::Draw(Rectangle &rect, Context &context) {
    // circle
    circleCenter = (Vector2){rect.x + rect.height / 2, rect.y + rect.height / 2};
    radius = rect.height / 2;
    DrawCircle(rect.x + rect.height / 2 + Editor::SmallGap(), rect.y + rect.height / 2,
               rect.height / 2, GetColor());

    // label
    // const char *label = TextFormat("%s[%s]", name.c_str(), ShaderTypeToStringMap[type].c_str());
    const char *label = name.c_str();
    const float textWidth = MeasureTextEx(Editor::GetFont(), label, Editor::TextSizeF(), 1).x;
    rect.width = std::max(rect.width, textWidth + Editor::SmallGap() * 3 + rect.height);
    DrawTextEx(Editor::GetFont(), label,
               {rect.x + rect.height + Editor::SmallGap() * 2, rect.y},
               Editor::TextSizeF(), 1, WHITE);

    if (source == nullptr) {
        // type info
        if (type != ShaderType::Float) {
            const float typeWidth = MeasureTextEx(Editor::GetFont(), ShaderTypeToStringMap[type].c_str(),
                                                  Editor::TextSizeF(),
                                                  1).x;
            DrawTextEx(Editor::GetFont(), ShaderTypeToStringMap[type].c_str(),
                       {rect.x - typeWidth - Editor::SmallGap(), rect.y},
                       Editor::TextSizeF(), 1, GRAY);
        }
        // value box
        else if (context.camera.zoom > 0.5f) {
            EndMode2D();
            const float propWidth = 50;
            const Vector2 pos = GetWorldToScreen2D({rect.x, rect.y}, context.camera);
            Vector2 size = Vector2({propWidth, Editor::TextSizeF()}) * context.camera.zoom;
            size.y = std::max(size.y, Editor::TextSizeF());
            const Rectangle propRect = {
                pos.x - size.x - Editor::SmallGap(), pos.y, size.x, size.y
            };
            if (GuiValueBoxFloat(propRect, nullptr, floatBuffer, &floatValue, floatEditMode)) {
                floatEditMode = !floatEditMode;
                std::cout << "float value changed" << std::endl;
            }
            BeginMode2D(context.camera);
        }
    }
    rect.y += rect.height + Editor::MediumGap();

    // line
    if (source != nullptr) {
        const float handleLength = abs(std::max((circleCenter - source->circleCenter).x,
                                                (circleCenter - source->circleCenter).y)) / 2;
        RaylibUtils::DrawLineBezier(
            circleCenter, circleCenter + Vector2{-handleLength, 0},
            source->circleCenter + Vector2{handleLength, 0}, source->circleCenter,
            2, Fade(GREEN, 0.5f));
    }
}

void NodeInput::Update(Context &context) {
    const MouseDragState dragState = context.mouseDragState;
    const Vector2 startMouseWorldPos = GetScreenToWorld2D(dragState.startDragPosition, context.camera);
    const bool wasHovered = CheckCollisionPointCircle(startMouseWorldPos, circleCenter, radius);

    // handle hovering
    const bool isHovering = CheckCollisionPointCircle(context.mousePos, circleCenter, radius * 2.);
    if ((wasHovered || isHovering) && context.interactionStateLowerThan(InteractionState::IOHovering)) {
        context.interactionState = InteractionState::IOHovering;
        hovering = true;
    } else {
        hovering = false;
    }

    // handle dragging
    // if (dragState.JustFinishedDragging() && wasHovered
    //     && context.interactionStateLowerThan(InteractionState::Connecting)) {
    //     context.interactionState = InteractionState::Connecting;
    // }
}

void NodeInput::Resolve(Context &context) {
    const bool isHovering = CheckCollisionPointCircle(context.mousePos, circleCenter, radius * 2.);

    // resolve being connected
    if (context.mouseDragState.JustFinishedDragging() && context.interactionState == InteractionState::Connecting &&
        isHovering) {
        if (context.connectionOutput != nullptr) {
            context.connectionOutput->target = this;
            source = context.connectionOutput;
        }
    }
}

void NodeInput::OnEditorGUI(Rectangle &rect) {
    Rectangle dropdownRect = {rect.x, rect.y, rect.width / 2, Editor::TextSize() * 1.0f};
    typeDropdown.OnEditorGUI(dropdownRect);
    if (typeDropdown.justSelected && !typeDropdown.editMode) {
        typeDropdown.justSelected = false;
        type = ShaderTypeMap[typeDropdown.GetSelectedOption()];
    }

    if (GuiTextBox(Rectangle{rect.x + dropdownRect.width, rect.y, rect.width / 2, Editor::TextSize() * 1.5f},
                   const_cast<char *>(name.c_str()), 256, nameEditMode)) {
        nameEditMode = !nameEditMode;
    }

    rect.y += typeDropdown.GetEditorHeight();

    if (source != nullptr) {
        GuiLabel({rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f},
                 TextFormat("Connected to: %s from %s", source->name.c_str(), source->parentNode->name.c_str()));
        rect.y += Editor::TextSize() * 1.0f;
    }
}

std::string NodeInput::GetVarName() const {
    return "input_" + std::to_string(parentNode->uid) + "_" + name;
}
