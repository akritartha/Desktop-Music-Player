#ifndef ADDFOLDERPOPUP_H
#define ADDFOLDERPOPUP_H

#include "raylib.h"
#include <string>

enum AddFolderResult { ADDFOLDER_NONE, ADDFOLDER_CANCELLED, ADDFOLDER_ADDED };

void DrawAddFolderPopup(Font poppinsFont, Font poppinsFontBold,
                         std::string& folderPathText, bool* fieldActive,
                         Vector2 virtualMouse);
AddFolderResult GetAddFolderPopupResult(Vector2 virtualMouse);

#endif // ADDFOLDERPOPUP_H