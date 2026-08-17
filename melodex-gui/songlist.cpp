#include "songlist.h"
#include "raylib.h"
#include "textutils.h"
#include <cctype>
#include "songlistpanel.h"

#define FONT_SCALE 1.0f

static Texture2D magnifierTexture;

void LoadSongListAssets() {
    magnifierTexture = LoadTexture("melodex-gui/icons/magnifier.png");
    SetTextureFilter(magnifierTexture, TEXTURE_FILTER_BILINEAR);
}

void UnloadSongListAssets() {
    UnloadTexture(magnifierTexture);
}

int DrawSongList(Font poppinsFont, Font poppinsFontBold, float* scrollOffset, Vector2 virtualMouse, const std::vector<SongEntry>& songs, int currentSongIndex, const std::string& searchText, const std::vector<int>& activePlaylistSongIndices, bool showAllSongs, int* rightClickedSongIndex, Rectangle* rightClickedRowRec) {
    Color bgPanelColor = { 255, 255, 255, 76 };

    // 1. Left box background panel
    Rectangle leftPanelRec = { 50.0f, 120.0f, 639.0f, 720.0f };
    DrawRectangleRounded(leftPanelRec, 0.08f, 8, bgPanelColor);

    // 2. Search bar background
    Rectangle searchRec = { 82.0f, 148.0f, 569.0f, 60.0f };
    DrawRectangleRounded(searchRec, 0.25f, 8, bgPanelColor);

    // 3. Search placeholder text
    Vector2 searchPos = { 150.0f, 165.0f };
    Color placeholderColor = { 255, 255, 255, 220 };
    if (!searchText.empty()) {
        DrawTextEx(poppinsFont, searchText.c_str(), searchPos, 25.0f * FONT_SCALE, 1.0f, WHITE);
    } else {
        DrawTextEx(poppinsFont, "Search for Songs", searchPos, 25.0f * FONT_SCALE, 1.0f, placeholderColor);
    }

    Rectangle magSource = { 0, 0, (float)magnifierTexture.width, (float)magnifierTexture.height };
    Rectangle magDest = { 98.0f, 163.0f, 30.0f, 30.0f };
    DrawTexturePro(magnifierTexture, magSource, magDest, (Vector2){0, 0}, 0.0f, WHITE);


    float listAreaTop = 245.0f;
    float listAreaBottom = leftPanelRec.y + leftPanelRec.height;

    Rectangle clipRec = { leftPanelRec.x, listAreaTop, leftPanelRec.width, listAreaBottom - listAreaTop };
    
    SongListPanel panel(clipRec, poppinsFont, poppinsFontBold, virtualMouse, songs, currentSongIndex, searchText, activePlaylistSongIndices, showAllSongs, rightClickedSongIndex, rightClickedRowRec);
    panel.SetScrollOffset(*scrollOffset);
    panel.Draw();
    
    *scrollOffset = panel.GetScrollOffset();
    return panel.GetClickedIndex();
}

SongEntry::SongEntry() : m_title(""), m_artist(""), m_thumbnailPath("") {}

SongEntry::SongEntry(std::string title, std::string artist, std::string thumbnailPath)
    : m_title(title), m_artist(artist), m_thumbnailPath(thumbnailPath) {}

std::string SongEntry::title() const { return m_title; }
std::string SongEntry::artist() const { return m_artist; }
std::string SongEntry::thumbnailPath() const { return m_thumbnailPath; }

void SongEntry::setTitle(std::string title) { m_title = title; }
void SongEntry::setArtist(std::string artist) { m_artist = artist; }
void SongEntry::setThumbnailPath(std::string path) { m_thumbnailPath = path; }