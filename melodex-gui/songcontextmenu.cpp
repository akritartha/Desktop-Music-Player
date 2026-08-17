#include "songcontextmenu.h"
#include "raylib.h"

namespace {
    Rectangle CalculateMenuBounds(Rectangle anchorRec, bool showRemoveOption, bool showSubmenu, size_t playlistNamesSize) {
        float menuX = anchorRec.x + anchorRec.width + 5.0f;
        float menuY = anchorRec.y;
        float menuWidth = 220.0f;
        float menuHeight = 100.0f;
        if (showRemoveOption) {
            menuHeight += 40.0f;
        }
        if (showSubmenu) {
            menuHeight += playlistNamesSize * 40.0f;
        }
        return { menuX, menuY, menuWidth, menuHeight };
    }
}

SongContextMenu::SongContextMenu(Rectangle anchorRec, Font font, bool isFav, bool showSub, 
                                 const std::vector<std::string>& playlistNames, Vector2 mouse, bool showRemove)
    : UIComponent(CalculateMenuBounds(anchorRec, showRemove, showSub, playlistNames.size())),
      poppinsFont(font), showSubmenu(showSub), isFavorite(isFav), userPlaylistNames(playlistNames), 
      virtualMouse(mouse), showRemoveOption(showRemove) {}

void SongContextMenu::Draw() {
    // 2. Background
    DrawRectangleRounded(bounds, 0.1f, 8, {30, 45, 70, 240});

    // 3. Menu item 1
    const char* favText = isFavorite ? "Remove from Favorites" : "Add to Favorites";
    DrawTextEx(poppinsFont, favText, {bounds.x + 15.0f, bounds.y + 15.0f}, 16.0f, 1.0f, WHITE);
    
    // 4. Menu item 2
    DrawTextEx(poppinsFont, "Add to Playlist >", {bounds.x + 15.0f, bounds.y + 55.0f}, 16.0f, 1.0f, WHITE);
    
    // 4b. Remove from playlist (only shown when viewing a specific playlist)
    float nextY = bounds.y + 80.0f;
    if (showRemoveOption) {
        DrawTextEx(poppinsFont, "Remove from Playlist", {bounds.x + 15.0f, nextY + 12.0f}, 16.0f, 1.0f, WHITE);
        nextY += 40.0f;
    }

    // 5. Submenu items
    if (showSubmenu) {
        float subY = nextY;
        for (size_t i = 0; i < userPlaylistNames.size(); ++i) {
            DrawTextEx(poppinsFont, userPlaylistNames[i].c_str(), {bounds.x + 15.0f, subY + 12.0f}, 16.0f, 1.0f, WHITE);
            subY += 40.0f;
        }
    }
}

ContextMenuAction SongContextMenu::GetAction() {
    Rectangle favBtnRec = { bounds.x, bounds.y, bounds.width, 40.0f };
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, favBtnRec)) {
        return { CTX_TOGGLE_FAVORITE, -1 };
    }

    Rectangle addBtnRec = { bounds.x, bounds.y + 40.0f, bounds.width, 40.0f };
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, addBtnRec)) {
        return { CTX_ADD_TO_PLAYLIST_SUBMENU, -1 };
    }

    float nextY = bounds.y + 80.0f;
    if (showRemoveOption) {
        Rectangle removeBtnRec = { bounds.x, nextY, bounds.width, 40.0f };
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, removeBtnRec)) {
            return { CTX_REMOVE_FROM_PLAYLIST, -1 };
        }
        nextY += 40.0f;
    }

    if (showSubmenu) {
        float subY = nextY;
        for (size_t i = 0; i < userPlaylistNames.size(); ++i) {
            Rectangle rowRec = { bounds.x, subY, bounds.width, 40.0f };
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, rowRec)) {
                return { CTX_PLAYLIST_SELECTED, (int)i };
            }
            subY += 40.0f;
        }
    }

    // 6. Outside click to close
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !CheckCollisionPointRec(virtualMouse, bounds)) {
        return { CTX_CLOSED, -1 };
    }

    // 7. Default
    return { CTX_NONE, -1 };
}

ContextMenuAction DrawSongContextMenu(Font poppinsFont, Rectangle anchorRowRec, 
                                        bool isFavorite, bool showSubmenu,
                                        const std::vector<std::string>& userPlaylistNames,
                                        Vector2 virtualMouse,
                                        bool showRemoveOption) {
    SongContextMenu menu(anchorRowRec, poppinsFont, isFavorite, showSubmenu, userPlaylistNames, virtualMouse, showRemoveOption);
    menu.Draw();
    return menu.GetAction();
}
