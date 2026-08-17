#include "addfolderpopup.h"
#include "raylib.h"

AddFolderPopup::AddFolderPopup(Rectangle b, Font poppins, Font poppinsBold, std::string* path, bool* active, Vector2 mouse)
    : Popup(b), poppinsFont(poppins), poppinsFontBold(poppinsBold), folderPathText(path), fieldActive(active), virtualMouse(mouse) {}

AddFolderPopup::AddFolderPopup(Rectangle b)
    : Popup(b), folderPathText(nullptr), fieldActive(nullptr) {}

void AddFolderPopup::Draw() {
    DrawOverlay();
    DrawRectangleRounded(bounds, 0.08f, 8, {40, 60, 90, 255});
    DrawTextEx(poppinsFontBold, "Add Music Folder", {bounds.x + 40, bounds.y + 30}, 32, 1.0f, WHITE);
    DrawTextEx(poppinsFont, "Folder Path", {bounds.x + 40, bounds.y + 90}, 18, 1.0f, WHITE);

    Rectangle pathBoxRec = { bounds.x + 40, bounds.y + 120, 520, 50 };
    DrawRectangleRounded(pathBoxRec, 0.2f, 8, {255, 255, 255, 40});

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *fieldActive = CheckCollisionPointRec(virtualMouse, pathBoxRec);
    }

    if (*fieldActive) {
        DrawRectangleRoundedLinesEx(pathBoxRec, 0.2f, 8, 2, WHITE);

        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 125 && folderPathText->length() < 200) {
                *folderPathText += (char)key;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !folderPathText->empty()) {
            folderPathText->pop_back();
        }
        if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_V)) {
            const char* clip = GetClipboardText();
            if (clip != nullptr) {
                *folderPathText += clip;
                if (folderPathText->length() > 120) *folderPathText = folderPathText->substr(0, 120);
            }
        }
    }

    if (folderPathText->empty()) {
        DrawTextEx(poppinsFont, "e.g. /home/user/Music/MoreSongs", {bounds.x + 55, bounds.y + 135}, 20, 1.0f, {255, 255, 255, 120});
    } else {
        DrawTextEx(poppinsFont, folderPathText->c_str(), {bounds.x + 55, bounds.y + 135}, 20, 1.0f, WHITE);
    }

    Rectangle cancelBtnRec = { bounds.x + 40, bounds.y + 200, 240, 60 };
    DrawRectangleRoundedLinesEx(cancelBtnRec, 0.3f, 8, 2, WHITE);
    DrawTextEx(poppinsFontBold, "Cancel", {bounds.x + 120, bounds.y + 218}, 22, 1.0f, WHITE);

    Rectangle addBtnRec = { bounds.x + 320, bounds.y + 200, 240, 60 };
    DrawRectangleRounded(addBtnRec, 0.3f, 8, WHITE);
    DrawTextEx(poppinsFontBold, "Scan Folder", {bounds.x + 360, bounds.y + 218}, 22, 1.0f, {20, 40, 70, 255});
}

AddFolderResult AddFolderPopup::GetResult(Vector2 mouse) {
    Rectangle cancelBtnRec = { bounds.x + 40, bounds.y + 200, 240, 60 };
    Rectangle addBtnRec = { bounds.x + 320, bounds.y + 200, 240, 60 };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, cancelBtnRec)) {
            return ADDFOLDER_CANCELLED;
        }
        if (CheckCollisionPointRec(mouse, addBtnRec)) {
            return ADDFOLDER_ADDED;
        }
    }
    return ADDFOLDER_NONE;
}

void DrawAddFolderPopup(Font poppinsFont, Font poppinsFontBold,
                         std::string& folderPathText, bool* fieldActive,
                         Vector2 virtualMouse) {
    AddFolderPopup popup({660, 400, 600, 280}, poppinsFont, poppinsFontBold, &folderPathText, fieldActive, virtualMouse);
    popup.Draw();
}

AddFolderResult GetAddFolderPopupResult(Vector2 virtualMouse) {
    AddFolderPopup popup({660, 400, 600, 280});
    return popup.GetResult(virtualMouse);
}