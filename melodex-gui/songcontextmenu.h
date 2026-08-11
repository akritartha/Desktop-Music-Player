#ifndef SONGCONTEXTMENU_H
#define SONGCONTEXTMENU_H

#include "raylib.h"
#include <vector>
#include <string>

enum ContextMenuResult { CTX_NONE, CTX_TOGGLE_FAVORITE, CTX_ADD_TO_PLAYLIST_SUBMENU, CTX_PLAYLIST_SELECTED, CTX_REMOVE_FROM_PLAYLIST,  CTX_CLOSED };

struct ContextMenuAction {
    ContextMenuResult result;
    int selectedPlaylistIndex;  // only relevant if result == CTX_PLAYLIST_SELECTED
};

ContextMenuAction DrawSongContextMenu(Font poppinsFont, Rectangle anchorRowRec, 
                                        bool isFavorite, bool showSubmenu,
                                        const std::vector<std::string>& userPlaylistNames,
                                        Vector2 virtualMouse,
                                        bool showRemoveOption);

#endif // SONGCONTEXTMENU_H
