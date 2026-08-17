#include "confirmdialog.h"

ConfirmDialog::ConfirmDialog(Rectangle b, Font font, Font fontBold, const std::string& msg, Vector2 mouse)
    : Popup(b), poppinsFont(font), poppinsFontBold(fontBold), message(msg), virtualMouse(mouse) {}

void ConfirmDialog::Draw() {
    DrawOverlay();
    DrawRectangleRounded(bounds, 0.1f, 8, (Color){0, 31, 62, 240});
    DrawTextEx(poppinsFont, message.c_str(), {bounds.x + 20.0f, bounds.y + 20.0f}, 20.0f, 1.0f, WHITE);

    Rectangle yesBtn = {bounds.x + 40.0f, bounds.y + 90.0f, 140.0f, 45.0f};
    Rectangle noBtn = {bounds.x + 220.0f, bounds.y + 90.0f, 140.0f, 45.0f};

    DrawRectangleRounded(yesBtn, 0.2f, 8, (Color){0, 115, 230, 255});
    DrawTextEx(poppinsFont, "Delete", {yesBtn.x + 35.0f, yesBtn.y + 12.0f}, 18.0f, 1.0f, WHITE);

    DrawRectangleRounded(noBtn, 0.2f, 8, (Color){60, 90, 130, 255});
    DrawTextEx(poppinsFont, "Cancel", {noBtn.x + 35.0f, noBtn.y + 12.0f}, 18.0f, 1.0f, WHITE);
}

ConfirmResult ConfirmDialog::GetResult() const {
    Rectangle yesBtn = {bounds.x + 40.0f, bounds.y + 90.0f, 140.0f, 45.0f};
    Rectangle noBtn = {bounds.x + 220.0f, bounds.y + 90.0f, 140.0f, 45.0f};

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (CheckCollisionPointRec(virtualMouse, yesBtn)) return CONFIRM_YES;
        if (CheckCollisionPointRec(virtualMouse, noBtn)) return CONFIRM_NO;
    }
    return CONFIRM_NONE;
}
