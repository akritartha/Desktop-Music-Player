#include "createplaylistpopup.h"
#include "raylib.h"

void DrawCreatePlaylistPopup(Font poppinsFont, Font poppinsFontBold, 
                              std::string& nameText, std::string& coverPathText, 
                              bool* nameFieldActive, bool* coverFieldActive,
                              Vector2 virtualMouse) {
    // Semi-transparent black overlay
    DrawRectangle(0, 0, 1920, 1080, {0, 0, 0, 150});

    // Centered popup panel
    Rectangle popupRec = { 660, 340, 600, 400 };
    DrawRectangleRounded(popupRec, 0.08f, 8, {40, 60, 90, 255});

    // Title
    DrawTextEx(poppinsFontBold, "Create New Playlist", {700, 370}, 32, 1.0f, WHITE);

    // Name Label
    DrawTextEx(poppinsFont, "Name", {700, 440}, 18, 1.0f, WHITE);

    // Name Input Box
    Rectangle nameBoxRec = { 700, 470, 520, 50 };
    DrawRectangleRounded(nameBoxRec, 0.2f, 8, {255, 255, 255, 40});

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(virtualMouse, nameBoxRec)) {
            *nameFieldActive = true;
            *coverFieldActive = false;
        }
    }

    if (*nameFieldActive) {
    DrawRectangleRoundedLinesEx(nameBoxRec, 0.2f, 8, 2, WHITE);
    
    int key = GetCharPressed();
    while (key > 0) {
        if (key >= 32 && key <= 125 && nameText.length() < 60) {
            nameText += (char)key;
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !nameText.empty()) {
        nameText.pop_back();
    }
    if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_V)) {
        const char* clip = GetClipboardText();
        if (clip != nullptr) {
            nameText += clip;
            if (nameText.length() > 60) nameText = nameText.substr(0, 60);
        }
    }
}
    
    DrawTextEx(poppinsFont, nameText.c_str(), {715, 485}, 20, 1.0f, WHITE);

    // Cover Path Label
    DrawTextEx(poppinsFont, "Cover Image Path", {700, 545}, 18, 1.0f, WHITE);

    // Cover Path Input Box
    Rectangle coverBoxRec = { 700, 575, 520, 50 };
    DrawRectangleRounded(coverBoxRec, 0.2f, 8, {255, 255, 255, 40});

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(virtualMouse, coverBoxRec)) {
            *coverFieldActive = true;
            *nameFieldActive = false;
        }
    }

   if (*coverFieldActive) {
    DrawRectangleRoundedLinesEx(coverBoxRec, 0.2f, 8, 2, WHITE);
    
    int key = GetCharPressed();
    while (key > 0) {
        if (key >= 32 && key <= 125 && coverPathText.length() < 120) {
            coverPathText += (char)key;
        }
        key = GetCharPressed();
    }
    if (IsKeyPressed(KEY_BACKSPACE) && !coverPathText.empty()) {
        coverPathText.pop_back();
    }
    if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_V)) {
        const char* clip = GetClipboardText();
        if (clip != nullptr) {
            coverPathText += clip;
            if (coverPathText.length() > 120) coverPathText = coverPathText.substr(0, 120);
        }
    }
}
    if (coverPathText.empty()) {
        DrawTextEx(poppinsFont, "e.g. /home/user/Pictures/cover.jpg", {715, 590}, 20, 1.0f, {255, 255, 255, 120});
    } else {
        DrawTextEx(poppinsFont, coverPathText.c_str(), {715, 590}, 20, 1.0f, WHITE);
    }

    // Buttons
    Rectangle cancelBtnRec = { 700, 650, 240, 60 };
    DrawRectangleRoundedLinesEx(cancelBtnRec, 0.3f, 8, 2, WHITE);
    DrawTextEx(poppinsFontBold, "Cancel", {780, 668}, 22, 1.0f, WHITE);

    Rectangle createBtnRec = { 980, 650, 240, 60 };
    DrawRectangleRounded(createBtnRec, 0.3f, 8, WHITE);
    DrawTextEx(poppinsFontBold, "Create", {1060, 668}, 22, 1.0f, {20, 40, 70, 255});
}

PopupResult GetCreatePlaylistPopupResult(Vector2 virtualMouse) {
    Rectangle cancelBtnRec = { 700, 650, 240, 60 };
    Rectangle createBtnRec = { 980, 650, 240, 60 };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(virtualMouse, cancelBtnRec)) {
            return POPUP_CANCELLED;
        }
        if (CheckCollisionPointRec(virtualMouse, createBtnRec)) {
            return POPUP_CREATED;
        }
    }

    return POPUP_NONE;
}
