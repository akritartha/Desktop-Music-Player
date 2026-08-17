#include "playlistbox.h"
#include "raylib.h"
#include "textutils.h"
#include <cmath>
#include "playlistgrid.h"

#define FONT_SCALE 1.0f

int DrawPlaylistBox(Font poppinsFontBold, Vector2 virtualMouse, float* scrollOffset, std::vector<PlaylistEntry>& playlists, int* rightClickedPlaylistIndex) {
    Color bgPanelColor = { 255, 255, 255, 76 };

    // 1. Right box background panel
    Rectangle rightPanelRec = { 1230.0f, 120.0f, 630.0f, 720.0f };
    DrawRectangleRounded(rightPanelRec, 0.08f, 8, bgPanelColor);

    // 2. "Playlist" heading text
    Vector2 headingPos = { 1280.0f, 140.0f };
    DrawTextEx(poppinsFontBold, "Playlist", headingPos, 55.0f * FONT_SCALE, 1.5f, WHITE);
    Vector2 btnTextPos = { 1630.0f, 160.0f };
    DrawTextEx(poppinsFontBold, "+ Add Playlist", btnTextPos, 28.0f * FONT_SCALE, 1.0f, WHITE);

    Vector2 folderBtnTextPos = { 1630.0f, 195.0f };
    DrawTextEx(poppinsFontBold, "+ Add Folder", folderBtnTextPos, 24.0f * FONT_SCALE, 1.0f, WHITE);

    float listAreaTop = 250.0f;
    float listAreaBottom = rightPanelRec.y + rightPanelRec.height;
    Rectangle clipRec = { rightPanelRec.x, listAreaTop, rightPanelRec.width, listAreaBottom - listAreaTop };
    
    PlaylistGrid grid(clipRec, poppinsFontBold, virtualMouse, playlists, rightClickedPlaylistIndex);
    grid.SetScrollOffset(*scrollOffset);
    grid.Draw();
    
    *scrollOffset = grid.GetScrollOffset();
    return grid.GetClickedIndex();
}

bool IsAddPlaylistButtonClicked(Vector2 virtualMouse) {
    Rectangle btnRec = { 1620.0f, 155.0f, 190.0f, 40.0f };
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, btnRec)) {
        return true;
    }
    
    return false;
}

bool IsAddFolderButtonClicked(Vector2 virtualMouse) {
    Rectangle btnRec = { 1620.0f, 190.0f, 190.0f, 35.0f };
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, btnRec)) {
        return true;
    }

    return false;
}

PlaylistEntry::PlaylistEntry() : m_name(""), m_thumbnailPath(""), m_textureLoaded(false), m_isDefault(false) {
    m_thumbnailTexture = { 0 };
}

PlaylistEntry::PlaylistEntry(std::string name, std::string thumbnailPath, bool isDefault)
    : m_name(name), m_thumbnailPath(thumbnailPath), m_textureLoaded(false), m_isDefault(isDefault) {
    m_thumbnailTexture = { 0 };
}

std::string PlaylistEntry::name() const { return m_name; }
std::string PlaylistEntry::thumbnailPath() const { return m_thumbnailPath; }
Texture2D PlaylistEntry::thumbnailTexture() const { return m_thumbnailTexture; }
bool PlaylistEntry::textureLoaded() const { return m_textureLoaded; }
const std::vector<int>& PlaylistEntry::songIndices() const { return m_songIndices; }
bool PlaylistEntry::isDefault() const { return m_isDefault; }

void PlaylistEntry::setThumbnailTexture(Texture2D tex) { m_thumbnailTexture = tex; }
void PlaylistEntry::setTextureLoaded(bool loaded) { m_textureLoaded = loaded; }

void PlaylistEntry::addSongIndex(int idx) {
    m_songIndices.push_back(idx);
}

void PlaylistEntry::removeSongIndex(int idx) {
    for (auto it = m_songIndices.begin(); it != m_songIndices.end(); ++it) {
        if (*it == idx) {
            m_songIndices.erase(it);
            break;
        }
    }
}

void PlaylistEntry::setSongIndices(const std::vector<int>& indices) {
    m_songIndices = indices;
}