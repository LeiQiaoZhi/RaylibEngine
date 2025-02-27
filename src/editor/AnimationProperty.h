#ifndef ANIMATIONPROPERTY_H
#define ANIMATIONPROPERTY_H
#include <string>
#include "raylib.h"
#include "../utils/RaylibUtils.h"

#include "../GCM/Scene.h"


class AnimationProperty {
public:
    explicit AnimationProperty(Model *model, ModelAnimation *animation, int animationIndex, int *activeAnimationIndex)
        : model(model), animation(animation), animationIndex(animationIndex),
          activeAnimationIndex(activeAnimationIndex) {
    }

    void OnEditorGUI(Rectangle &rect);

    float GetEditorHeight() const;

    nlohmann::json ToJson() const;

    void FromJson(const nlohmann::json &json);

    void Update();

    void DrawGizmos(const Scene *scene, bool drawBonesLines, bool drawBoneNames, float radius) const;

private:
    // states
    Model *model = nullptr;
    ModelAnimation *animation = nullptr;
    int animationIndex = 0;
    int *activeAnimationIndex;

    // playback states
    int currentFrame = 0;
    bool isLooping = false;

    // gui
    float height;
    std::string statusText = "";
    bool statusWarning = false;
};


#endif //MATERIALPROPERTY_H
