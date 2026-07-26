#ifndef SONGLIST_H
#define SONGLIST_H

#include "raylib.h"
#include <string>
#include <vector>

struct SongEntry {
    std::string title;
    std::string artist;
    std::string thumbnailPath;  // path to image file, empty string if none
};

int DrawSongList(Font poppinsFont, Font poppinsFontBold, float* scrollOffset, Vector2 virtualMouse, const std::vector<SongEntry>& songs, int currentSongIndex, const std::string& searchText);
void LoadSongListAssets();
void UnloadSongListAssets();

#endif // SONGLIST_H
