#ifndef NodePropertiesSubView_H
#define NodePropertiesSubView_H
#include "../common/editor/DropdownProperty.h"
#include "../common/subviews/ScrollPanelRenderer.h"
#include "../common/GCM/Scene.h"
#include "../common/editor/FileHierarchyProperty.h"
#include "nodes/Node.h"

class NodePropertiesSubView {
public:
    NodePropertiesSubView(int width, int height)
        : nodesDropdown(DropdownProperty(GetOptions(), 0)) {
        renderer_ = new ScrollPanelRenderer(width, height - RAYGUI_WINDOWBOX_STATUSBAR_HEIGHT);
    }

    std::vector<std::string> GetOptions() {
        std::vector<std::string> options;
        const std::string path = INTERNAL_ASSET_DIR "/nodes";
        const FilePathList files = LoadDirectoryFilesEx(path.c_str(), NULL, false);
        for (int i = 0; i < files.count; i++) {
            const char *completeFilePath = files.paths[i];
            std::string file = GetFileName(completeFilePath);
            if (file[0] == '.' || !Utils::EndsWith(file, ".json"))
                continue;

            std::string name = file.erase(file.find(".json"));
            options.push_back(file);
        }
        UnloadDirectoryFiles(files);

        std::sort(options.begin(), options.end());

        return options;
    }

    void Render(Vector2 position, Context &context);

    Vector2 GetSize() const {
        return renderer_->GetSize();
    }

private:
    ISubviewRenderer *renderer_;
    DropdownProperty nodesDropdown;

    char graphFilename[32] = "test.graph.json";
    bool editMode;
    std::string statusText;
    bool statusWarning;
};


#endif //NodePropertiesSubView_H
