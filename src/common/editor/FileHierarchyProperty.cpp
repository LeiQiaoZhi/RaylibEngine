#include <raygui.h>
#include "FileHierarchyProperty.h"
#include "Editor.h"
#include "../utils/Utils.h"

void FileHierarchyProperty::OnEditorGUI(Rectangle &rect) {
    // Title
    const char *title = GetFileName(path.c_str());
    const bool isFile = IsPathFile(path.c_str());
    const int icon = isFile ? ICON_FILETYPE_TEXT : folded ? ICON_ARROW_RIGHT_FILL : ICON_ARROW_DOWN_FILL;
    title = GuiIconText(icon, title);

    // Title UI
    Rectangle titleRect = {rect.x, rect.y, rect.width, Editor::TextSize() * 1.0f};
    if (GuiLabelButton(titleRect, title)) {
        folded = !folded;
    }

    // Hover -- show action buttons
    if (CheckCollisionPointRec(GetMousePosition(), titleRect)) {
        const float buttonWidth = Editor::TextSize() * 1.2f;
        Rectangle endRect = {rect.x + rect.width - buttonWidth, rect.y, buttonWidth, Editor::TextSize() * 1.0f};
        if (GuiLabelButton(endRect, GuiIconText(ICON_FOLDER_OPEN, ""))) {
            Utils::OpenFileInFileExplorer(path);
        }
        endRect.x -= buttonWidth + Editor::SmallGap();
        if (isFile) {
            if (GuiLabelButton(endRect, GuiIconText(ICON_FILE_OPEN, ""))) {
                Utils::OpenFileWithDefaultApp(path);
            }
        }
    }
    rect.y += Editor::TextSize() + Editor::SmallGap();
    height = Editor::TextSize() + Editor::SmallGap();

    // Children
    if (folded || isFile)
        return;

    const FilePathList files = LoadDirectoryFilesEx(path.c_str(), NULL, false);
    for (int i = 0; i < files.count; i++) {
        const char *completeFilePath = files.paths[i];
        const char *file = GetFileName(completeFilePath);
        if (file[0] == '.')
            continue;

        if (children.find(completeFilePath) == children.end()) {
            children[completeFilePath] = std::make_unique<FileHierarchyProperty>(completeFilePath);
        }
        FileHierarchyProperty *fileHierarchyProperty = children[completeFilePath].get();

        float indent = Editor::SmallGap() * 4;
        rect.x += indent;
        rect.width -= indent;

        fileHierarchyProperty->OnEditorGUI(rect);

        rect.width += indent;
        rect.x -= indent;
        rect.height -= fileHierarchyProperty->GetEditorHeight();
        height += fileHierarchyProperty->GetEditorHeight();
    }
    UnloadDirectoryFiles(files);
}


float FileHierarchyProperty::GetEditorHeight() const {
    return height;
}
