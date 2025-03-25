#include <raygui.h>
#include "AnimationProperty.h"
#include "Editor.h"
#include "rlgl.h"
#include "../utils/RaylibUtils.h"

void AnimationProperty::OnEditorGUI(Rectangle &rect) {
    const float originalY = rect.y;

    const char *label = TextFormat("[%i] [%s]", animationIndex, animation->name);
    const float labelWidth = Editor::TextWidth(label) + Editor::SmallGap();
    GuiLabel({rect.x, rect.y, labelWidth, Editor::TextSize() * 1.0f}, label);

    // info
    Editor::BeginIndent(rect, labelWidth + Editor::SmallGap());
    const float halfWidth = rect.width / 2;
    const char *boneCountLabel = TextFormat("Bones: %i", animation->boneCount);
    GuiLabel({rect.x, rect.y, halfWidth, Editor::TextSize() * 1.0f}, boneCountLabel);
    const char *frameCountLabel = TextFormat("Frames: %i/%i", currentFrame, animation->frameCount);
    GuiLabel({rect.x + halfWidth, rect.y, halfWidth, Editor::TextSize() * 1.0f}, frameCountLabel);
    Editor::EndIndent(rect, labelWidth + Editor::SmallGap());
    rect.y += Editor::TextSize() + Editor::SmallGap();

    // playback
    const float lineHeight = Editor::TextSize() + Editor::SmallGap();
    const bool playing = *activeAnimationIndex == animationIndex;
    const char *playLabel = GuiIconText(playing ? ICON_PLAYER_PAUSE : ICON_PLAYER_PLAY, "");
    const float playWidth = Editor::TextWidth(playLabel);
    if (GuiButton({rect.x, rect.y, playWidth, lineHeight}, playLabel)) {
        *activeAnimationIndex = playing ? -1 : animationIndex;
    }
    Editor::BeginIndent(rect, playWidth);
    const char *resetLabel = GuiIconText(ICON_PLAYER_PREVIOUS, "");
    const float resetWidth = Editor::TextWidth(resetLabel);
    if (GuiButton({rect.x, rect.y, resetWidth, lineHeight}, resetLabel)) {
        currentFrame = 0;
    }
    Editor::BeginIndent(rect, resetWidth);
    const char *loopLabel = GuiIconText(ICON_RESTART, "");
    const float loopWidth = Editor::TextWidth(loopLabel);
    Color originalButtonColor =
            Editor::BeginButtonColor(isLooping ? Editor::FocusedColor() : Editor::BackgroundColor());
    if (GuiButton({rect.x, rect.y, loopWidth, lineHeight}, loopLabel)) {
        isLooping = !isLooping;
    }
    Editor::EndButtonColor(originalButtonColor);
    Editor::BeginIndent(rect, loopWidth);
    float currentFrameFloat = static_cast<float>(currentFrame);
    if (GuiSliderBar({rect.x, rect.y, rect.width, lineHeight}, NULL, NULL, &currentFrameFloat, 0,
                     animation->frameCount - 1)) {
        *activeAnimationIndex = animationIndex;
        currentFrame = static_cast<int>(currentFrameFloat);
    }
    Editor::EndIndent(rect, loopWidth);
    Editor::EndIndent(rect, resetWidth);
    Editor::EndIndent(rect, playWidth);
    rect.y += lineHeight + Editor::SmallGap();

    // status
    Editor::DrawStatusInfoBox(rect, statusText, statusWarning);

    height = rect.y - originalY;
}

float AnimationProperty::GetEditorHeight() const {
    return height;
}

void AnimationProperty::Update() {
    currentFrame++;
    if (isLooping)
        currentFrame %= animation->frameCount;
    else
        currentFrame = std::min(currentFrame, animation->frameCount - 1);
    // UpdateModelAnimation(*model, *animation, currentFrame);
    UpdateModelAnimationBones(*model, *animation, currentFrame);
}

void AnimationProperty::DrawGizmos(const Scene *scene, bool drawBonesLines, bool drawBoneNames, float radius) const {
    const Camera *camera = scene->GetMainCamera()->GetRaylibCamera();
    // bones -- when playing animation
    if (drawBonesLines) {
        rlDisableDepthTest();
        for (int i = 0; i < animation->boneCount; i++) {
            const BoneInfo bone = animation->bones[i];
            const Transform boneTransform = animation->framePoses[currentFrame][i];
            const Vector3 bonePosition = Vector3Transform(boneTransform.translation, model->transform);
            if (bone.parent >= 0 && bone.parent < model->boneCount) {
                const Transform parentBoneTransform = animation->framePoses[currentFrame][bone.parent];
                const Vector3 parentBonePosition = Vector3Transform(parentBoneTransform.translation, model->transform);
                DrawLine3D(bonePosition, parentBonePosition, ORANGE);
            }
            // DrawSphere(bonePosition, radius, ORANGE);
        }
    }

    if (drawBoneNames) {
        EndMode3D();
        for (int i = 0; i < animation->boneCount; i++) {
            const BoneInfo bone = animation->bones[i];
            const Transform boneTransform = animation->framePoses[currentFrame][i];
            const Vector3 bonePosition = Vector3Transform(boneTransform.translation, model->transform);

            const float padding = 5;
            const float width = MeasureText(bone.name, 10);
            Vector2 screenPosition = GetWorldToScreenEx(bonePosition, *camera, scene->screenSpaceRect.width,
                                                        scene->screenSpaceRect.height);
            screenPosition += Vector2{radius, -radius}; // offset a bit
            DrawRectangle(screenPosition.x - padding, screenPosition.y - padding,
                          width + padding * 2, 10 + padding * 2, Fade(BLACK, 0.5f));
            DrawText(bone.name, screenPosition.x, screenPosition.y, 10, WHITE);
        }
        BeginMode3D(*camera);
        rlEnableDepthTest();
    }
}

nlohmann::json AnimationProperty::ToJson() const {
    nlohmann::json j;
    return j;
}

void AnimationProperty::FromJson(const nlohmann::json &json) {
}
