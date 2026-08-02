#include "songcontextmenu.h"
#include "raylib.h"

ContextMenuAction DrawSongContextMenu(Font poppinsFont, Rectangle anchorRowRec, 
                                        bool isFavorite, bool showSubmenu,
                                        const std::vector<std::string>& userPlaylistNames,
                                        Vector2 virtualMouse) {
    float menuX = anchorRowRec.x + anchorRowRec.width + 5.0f;
    float menuY = anchorRowRec.y;
    float menuWidth = 220.0f;
    float menuHeight = 100.0f;
    
    if (showSubmenu) {
        menuHeight += userPlaylistNames.size() * 40.0f;
    }

    Rectangle menuRec = { menuX, menuY, menuWidth, menuHeight };

    // 2. Background
    DrawRectangleRounded(menuRec, 0.1f, 8, {30, 45, 70, 240});

    // 3. Menu item 1
    const char* favText = isFavorite ? "Remove from Favorites" : "Add to Favorites";
    DrawTextEx(poppinsFont, favText, {menuX + 15.0f, menuY + 15.0f}, 16.0f, 1.0f, WHITE);
    
    Rectangle favBtnRec = { menuX, menuY, menuWidth, 40.0f };
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, favBtnRec)) {
        return { CTX_TOGGLE_FAVORITE, -1 };
    }

    // 4. Menu item 2
    DrawTextEx(poppinsFont, "Add to Playlist >", {menuX + 15.0f, menuY + 55.0f}, 16.0f, 1.0f, WHITE);
    
    Rectangle addBtnRec = { menuX, menuY + 40.0f, menuWidth, 40.0f };
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, addBtnRec)) {
        return { CTX_ADD_TO_PLAYLIST_SUBMENU, -1 };
    }

    // 5. Submenu items
    if (showSubmenu) {
        float subY = menuY + 80.0f;
        for (size_t i = 0; i < userPlaylistNames.size(); ++i) {
            DrawTextEx(poppinsFont, userPlaylistNames[i].c_str(), {menuX + 15.0f, subY + 12.0f}, 16.0f, 1.0f, WHITE);
            
            Rectangle rowRec = { menuX, subY, menuWidth, 40.0f };
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, rowRec)) {
                return { CTX_PLAYLIST_SELECTED, (int)i };
            }
            subY += 40.0f;
        }
    }

    // 6. Outside click to close
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !CheckCollisionPointRec(virtualMouse, menuRec)) {
        return { CTX_CLOSED, -1 };
    }

    // 7. Default
    return { CTX_NONE, -1 };
}
