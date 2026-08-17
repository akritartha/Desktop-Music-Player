#ifndef CREATEPLAYLISTPOPUP_H
#define CREATEPLAYLISTPOPUP_H

#include "popup.h"
#include <string>

enum PopupResult {
    POPUP_NONE = 0,
    POPUP_CANCELLED,
    POPUP_CREATED
};

class CreatePlaylistPopup : public Popup {
private:
    Font poppinsFont;
    Font poppinsFontBold;
    std::string* nameText;
    std::string* coverPathText;
    bool* nameFieldActive;
    bool* coverFieldActive;
    Vector2 virtualMouse;

public:
    CreatePlaylistPopup(Rectangle b, Font poppins, Font poppinsBold, std::string* name, std::string* cover, bool* nameActive, bool* coverActive, Vector2 mouse);
    CreatePlaylistPopup(Rectangle b);
    void Draw() override;
    PopupResult GetResult(Vector2 mouse);
};

void DrawCreatePlaylistPopup(Font poppinsFont, Font poppinsFontBold, 
                              std::string& nameText, std::string& coverPathText, 
                              bool* nameFieldActive, bool* coverFieldActive,
                              Vector2 virtualMouse);

PopupResult GetCreatePlaylistPopupResult(Vector2 virtualMouse);

#endif // CREATEPLAYLISTPOPUP_H
