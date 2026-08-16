#include "addfolderpopup.h"
#include "raylib.h"

void DrawAddFolderPopup(Font poppinsFont, Font poppinsFontBold,
                         std::string& folderPathText, bool* fieldActive,
                         Vector2 virtualMouse) {
    // Semi-transparent black overlay
    DrawRectangle(0, 0, 1920, 1080, {0, 0, 0, 150});

    // Centered popup panel
    Rectangle popupRec = { 660, 400, 600, 280 };
    DrawRectangleRounded(popupRec, 0.08f, 8, {40, 60, 90, 255});

    // Title
    DrawTextEx(poppinsFontBold, "Add Music Folder", {700, 430}, 32, 1.0f, WHITE);

    // Folder path label
    DrawTextEx(poppinsFont, "Folder Path", {700, 490}, 18, 1.0f, WHITE);

    // Folder path input box
    Rectangle pathBoxRec = { 700, 520, 520, 50 };
    DrawRectangleRounded(pathBoxRec, 0.2f, 8, {255, 255, 255, 40});

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *fieldActive = CheckCollisionPointRec(virtualMouse, pathBoxRec);
    }

    if (*fieldActive) {
        DrawRectangleRoundedLinesEx(pathBoxRec, 0.2f, 8, 2, WHITE);

        int key = GetCharPressed();
        while (key > 0) {
            if (key >= 32 && key <= 125 && folderPathText.length() < 200) {
                folderPathText += (char)key;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !folderPathText.empty()) {
            folderPathText.pop_back();
        }
    }

    if (folderPathText.empty()) {
        DrawTextEx(poppinsFont, "e.g. /home/user/Music/MoreSongs", {715, 535}, 20, 1.0f, {255, 255, 255, 120});
    } else {
        DrawTextEx(poppinsFont, folderPathText.c_str(), {715, 535}, 20, 1.0f, WHITE);
    }

    // Buttons
    Rectangle cancelBtnRec = { 700, 600, 240, 60 };
    DrawRectangleRoundedLinesEx(cancelBtnRec, 0.3f, 8, 2, WHITE);
    DrawTextEx(poppinsFontBold, "Cancel", {780, 618}, 22, 1.0f, WHITE);

    Rectangle addBtnRec = { 980, 600, 240, 60 };
    DrawRectangleRounded(addBtnRec, 0.3f, 8, WHITE);
    DrawTextEx(poppinsFontBold, "Scan Folder", {1020, 618}, 22, 1.0f, {20, 40, 70, 255});
}

AddFolderResult GetAddFolderPopupResult(Vector2 virtualMouse) {
    Rectangle cancelBtnRec = { 700, 600, 240, 60 };
    Rectangle addBtnRec = { 980, 600, 240, 60 };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(virtualMouse, cancelBtnRec)) {
            return ADDFOLDER_CANCELLED;
        }
        if (CheckCollisionPointRec(virtualMouse, addBtnRec)) {
            return ADDFOLDER_ADDED;
        }
    }

    return ADDFOLDER_NONE;
}