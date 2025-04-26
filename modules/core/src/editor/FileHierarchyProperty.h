#ifndef FILEHIERARCHYPROPERTY_H
#define FILEHIERARCHYPROPERTY_H
#include <map>
#include <string>

#include "raylib.h"
#include "../utils/RaylibUtils.h"


class FileHierarchyProperty {
public:
    explicit FileHierarchyProperty(std::string path, bool folded = true) : path(std::move(path)), folded(folded) {
    }

    void OnEditorGUI(Rectangle &rect);

    float GetEditorHeight() const;

    bool IsFolded() const {
        return folded;
    }

    void SetPath(std::string path) {
        this->path = std::move(path);
    }

private:
    std::string path;
    bool folded;
    int height;
    std::map<std::string, std::unique_ptr<FileHierarchyProperty> > children;
};


#endif //FILEHIERARCHYPROPERTY_H
