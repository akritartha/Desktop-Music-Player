#ifndef PLAYLISTBOX_H
#define PLAYLISTBOX_H

#include "raylib.h"
#include <string>
#include <vector>

struct PlaylistEntry {
    std::string name;
    std::string thumbnailPath;
    Texture2D thumbnailTexture = { 0 };
    bool textureLoaded = false;
    std::vector<int> songIndices;
    bool isDefault = false;
};

int DrawPlaylistBox(Font poppinsFontBold, Vector2 virtualMouse, float* scrollOffset, std::vector<PlaylistEntry>& playlists);
bool IsAddPlaylistButtonClicked(Vector2 virtualMouse);

#endif // PLAYLISTBOX_H
