#ifndef SONGLIST_H
#define SONGLIST_H

#include "raylib.h"
#include <string>
#include <vector>

class SongEntry {
private:
    std::string m_title;
    std::string m_artist;
    std::string m_thumbnailPath;
public:
    SongEntry();
    SongEntry(std::string title, std::string artist, std::string thumbnailPath = "");
    std::string title() const;
    std::string artist() const;
    std::string thumbnailPath() const;
    void setTitle(std::string title);
    void setArtist(std::string artist);
    void setThumbnailPath(std::string path);
};

int DrawSongList(Font poppinsFont, Font poppinsFontBold, float* scrollOffset, Vector2 virtualMouse, const std::vector<SongEntry>& songs, int currentSongIndex, const std::string& searchText, const std::vector<int>& activePlaylistSongIndices, bool showAllSongs, int* rightClickedSongIndex, Rectangle* rightClickedRowRec);
void LoadSongListAssets();
void UnloadSongListAssets();

#endif // SONGLIST_H
