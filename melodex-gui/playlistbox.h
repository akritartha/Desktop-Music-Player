#ifndef PLAYLISTBOX_H
#define PLAYLISTBOX_H

#include "raylib.h"
#include <string>
#include <vector>

struct PlaylistEntry {
    std::string name;
    std::string thumbnailPath;  // empty string if none
};

int DrawPlaylistBox(Font poppinsFontBold, Vector2 virtualMouse, float* scrollOffset, const std::vector<PlaylistEntry>& playlists);

#endif // PLAYLISTBOX_H
