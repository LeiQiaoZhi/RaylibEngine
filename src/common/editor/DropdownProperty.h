#ifndef DROPDOWNPROPERTY_H
#define DROPDOWNPROPERTY_H
#include <sstream>
#include <string>

#include "raylib.h"
#include "../utils/RaylibUtils.h"


class DropdownProperty {
public:
    explicit DropdownProperty(std::vector<std::string> options, int selectedIndex = 0) : options(std::move(options)),
        selectedIndex(selectedIndex) {
        std::ostringstream oss;
        for (size_t i = 0; i < this->options.size(); ++i) {
            if (i > 0) oss << ";";
            oss << this->options[i];
        }
        optionsStr = oss.str();
    }

    void OnEditorGUI(Rectangle &rect);

    float GetEditorHeight() const;

    nlohmann::json ToJson() const;

    void FromJson(const nlohmann::json &json);

    std::string GetSelectedOption() const {
        return options[selectedIndex];
    }

    int GetSelectedIndex() const {
        return selectedIndex;
    }

private:
    // States
    std::vector<std::string> options;
    int selectedIndex = 0;

    // UI
    std::string optionsStr;
    float height = 0;
    bool editMode = false;
};


#endif //MATERIALPROPERTY_H
