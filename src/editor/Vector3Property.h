#ifndef VECTOR3PROPERTY_H
#define VECTOR3PROPERTY_H
#include <string>

#include "raylib.h"
#include "../utils/RaylibUtils.h"


class Vector3Property {
public:
    explicit Vector3Property(Vector3 *vector3, std::string label) : label(std::move(label)), vector3(vector3) {
        for (int i = 0; i < 3; i++) {
            snprintf(valueBoxTextValues[i], 32, "%.2f", RaylibUtils::Vector3Get(*vector3, i));
        }
    }
    void OnEditorGUI(Rectangle& rect);
    float GetEditorHeight() const;

private:
    std::string label;
    std::string subLabels[3] = {"X", "Y", "Z"};
    Vector3 *vector3 = nullptr;
    bool editModes[3] = {false, false, false};
    char valueBoxTextValues[3][32] = {{0}, {0}, {0}};
};


#endif //VECTOR3PROPERTY_H
