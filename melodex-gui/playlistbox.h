#ifndef PLAYLISTBOX_H
#define PLAYLISTBOX_H

#include "raylib.h"
#include <string>
#include <vector>

class PlaylistEntry {
private:
    std::string m_name;
    std::string m_thumbnailPath;
    Texture2D m_thumbnailTexture = { 0 };
    bool m_textureLoaded = false;
    std::vector<int> m_songIndices;
    bool m_isDefault = false;
public:
    PlaylistEntry();
    PlaylistEntry(std::string name, std::string thumbnailPath = "", bool isDefault = false);
    std::string name() const;
    std::string thumbnailPath() const;
    Texture2D thumbnailTexture() const;
    bool textureLoaded() const;
    const std::vector<int>& songIndices() const;
    bool isDefault() const;
    void setThumbnailTexture(Texture2D tex);
    void setTextureLoaded(bool loaded);
    void addSongIndex(int idx);
    void removeSongIndex(int idx);
    void setSongIndices(const std::vector<int>& indices);
};

int DrawPlaylistBox(Font poppinsFontBold, Vector2 virtualMouse, float* scrollOffset, std::vector<PlaylistEntry>& playlists, int* rightClickedPlaylistIndex);
bool IsAddPlaylistButtonClicked(Vector2 virtualMouse);
bool IsAddFolderButtonClicked(Vector2 virtualMouse);

#endif // PLAYLISTBOX_H