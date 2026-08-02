#ifndef CREATEPLAYLISTPOPUP_H
#define CREATEPLAYLISTPOPUP_H

#include "raylib.h"
#include <string>

enum PopupResult { POPUP_NONE, POPUP_CANCELLED, POPUP_CREATED };

void DrawCreatePlaylistPopup(Font poppinsFont, Font poppinsFontBold, 
                              std::string& nameText, std::string& coverPathText, 
                              bool* nameFieldActive, bool* coverFieldActive,
                              Vector2 virtualMouse);
PopupResult GetCreatePlaylistPopupResult(Vector2 virtualMouse);

#endif // CREATEPLAYLISTPOPUP_H
