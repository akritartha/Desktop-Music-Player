#include "createplaylistpopup.h"
#include "raylib.h"

CreatePlaylistPopup::CreatePlaylistPopup(Rectangle b, Font poppins, Font poppinsBold, std::string* name, std::string* cover, bool* nameActive, bool* coverActive, Vector2 mouse)
    : Popup(b), poppinsFont(poppins), poppinsFontBold(poppinsBold), nameText(name), coverPathText(cover), nameFieldActive(nameActive), coverFieldActive(coverActive), virtualMouse(mouse) {}

CreatePlaylistPopup::CreatePlaylistPopup(Rectangle b)
    : Popup(b), nameText(nullptr), coverPathText(nullptr), nameFieldActive(nullptr), coverFieldActive(nullptr) {}

void CreatePlaylistPopup::Draw() {
    DrawOverlay();
    DrawRectangleRounded(bounds, 0.08f, 8, {40, 60, 90, 255});
    DrawTextEx(poppinsFontBold, "Create New Playlist", {bounds.x + 40, bounds.y + 30}, 32, 1.0f, WHITE);
    DrawTextEx(poppinsFont, "Name", {bounds.x + 40, bounds.y + 100}, 18, 1.0f, WHITE);

    Rectangle nameBoxRec = { bounds.x + 40, bounds.y + 130, 520, 50 };
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
            if (key >= 32 && key <= 125 && nameText->length() < 60) {
                *nameText += (char)key;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !nameText->empty()) {
            nameText->pop_back();
        }
        if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_V)) {
            const char* clip = GetClipboardText();
            if (clip != nullptr) {
                *nameText += clip;
                if (nameText->length() > 60) *nameText = nameText->substr(0, 60);
            }
        }
    }
    
    DrawTextEx(poppinsFont, nameText->c_str(), {bounds.x + 55, bounds.y + 145}, 20, 1.0f, WHITE);

    DrawTextEx(poppinsFont, "Cover Image Path", {bounds.x + 40, bounds.y + 205}, 18, 1.0f, WHITE);
    Rectangle coverBoxRec = { bounds.x + 40, bounds.y + 235, 520, 50 };
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
            if (key >= 32 && key <= 125 && coverPathText->length() < 120) {
                *coverPathText += (char)key;
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !coverPathText->empty()) {
            coverPathText->pop_back();
        }
        if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_V)) {
            const char* clip = GetClipboardText();
            if (clip != nullptr) {
                *coverPathText += clip;
                if (coverPathText->length() > 120) *coverPathText = coverPathText->substr(0, 120);
            }
        }
    }
    
    if (coverPathText->empty()) {
        DrawTextEx(poppinsFont, "e.g. /home/user/Pictures/cover.jpg", {bounds.x + 55, bounds.y + 250}, 20, 1.0f, {255, 255, 255, 120});
    } else {
        DrawTextEx(poppinsFont, coverPathText->c_str(), {bounds.x + 55, bounds.y + 250}, 20, 1.0f, WHITE);
    }

    Rectangle cancelBtnRec = { bounds.x + 40, bounds.y + 310, 240, 60 };
    DrawRectangleRoundedLinesEx(cancelBtnRec, 0.3f, 8, 2, WHITE);
    DrawTextEx(poppinsFontBold, "Cancel", {bounds.x + 120, bounds.y + 328}, 22, 1.0f, WHITE);

    Rectangle createBtnRec = { bounds.x + 320, bounds.y + 310, 240, 60 };
    DrawRectangleRounded(createBtnRec, 0.3f, 8, WHITE);
    DrawTextEx(poppinsFontBold, "Create", {bounds.x + 400, bounds.y + 328}, 22, 1.0f, {20, 40, 70, 255});
}

PopupResult CreatePlaylistPopup::GetResult(Vector2 mouse) {
    Rectangle cancelBtnRec = { bounds.x + 40, bounds.y + 310, 240, 60 };
    Rectangle createBtnRec = { bounds.x + 320, bounds.y + 310, 240, 60 };

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(mouse, cancelBtnRec)) {
            return POPUP_CANCELLED;
        }
        if (CheckCollisionPointRec(mouse, createBtnRec)) {
            return POPUP_CREATED;
        }
    }
    return POPUP_NONE;
}

void DrawCreatePlaylistPopup(Font poppinsFont, Font poppinsFontBold, 
                              std::string& nameText, std::string& coverPathText, 
                              bool* nameFieldActive, bool* coverFieldActive,
                              Vector2 virtualMouse) {
    CreatePlaylistPopup popup({660, 340, 600, 400}, poppinsFont, poppinsFontBold, &nameText, &coverPathText, nameFieldActive, coverFieldActive, virtualMouse);
    popup.Draw();
}

PopupResult GetCreatePlaylistPopupResult(Vector2 virtualMouse) {
    CreatePlaylistPopup popup({660, 340, 600, 400});
    return popup.GetResult(virtualMouse);
}
