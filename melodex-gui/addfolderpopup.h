#ifndef ADDFOLDERPOPUP_H
#define ADDFOLDERPOPUP_H

#include "popup.h"
#include <string>

enum AddFolderResult {
    ADDFOLDER_NONE = 0,
    ADDFOLDER_CANCELLED,
    ADDFOLDER_ADDED
};

class AddFolderPopup : public Popup {
private:
    Font poppinsFont;
    Font poppinsFontBold;
    std::string* folderPathText;
    bool* fieldActive;
    Vector2 virtualMouse;

public:
    AddFolderPopup(Rectangle b, Font poppins, Font poppinsBold, std::string* path, bool* active, Vector2 mouse);
    AddFolderPopup(Rectangle b);
    void Draw() override;
    AddFolderResult GetResult(Vector2 mouse);
};

void DrawAddFolderPopup(Font poppinsFont, Font poppinsFontBold,
                         std::string& folderPathText, bool* fieldActive,
                         Vector2 virtualMouse);

AddFolderResult GetAddFolderPopupResult(Vector2 virtualMouse);

#endif // ADDFOLDERPOPUP_H