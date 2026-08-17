#include "songlist.h"
#include "raylib.h"
#include "textutils.h"
#include <cctype>

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
    int clickedIndex = -1;
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

    bool mouseOverPanel = CheckCollisionPointRec(virtualMouse, leftPanelRec);
    if (mouseOverPanel) {
        float wheelMove = GetMouseWheelMove();
        *scrollOffset -= wheelMove * 30.0f;
    }

    std::vector<int> filteredIndices;
    for (size_t i = 0; i < songs.size(); ++i) {
        bool inPlaylist = showAllSongs;
        if (!showAllSongs) {
            for (int idx : activePlaylistSongIndices) {
                if (idx == (int)i) { inPlaylist = true; break; }
            }
        }
        if (!inPlaylist) continue;
        
        if (searchText.empty()) {
            filteredIndices.push_back((int)i);
        } else {
            std::string titleLower = songs[i].title();
            std::string artistLower = songs[i].artist();
            std::string searchLower = searchText;
            for (auto& c : titleLower) c = tolower(c);
            for (auto& c : artistLower) c = tolower(c);
            for (auto& c : searchLower) c = tolower(c);
            
            if (titleLower.find(searchLower) != std::string::npos || 
                artistLower.find(searchLower) != std::string::npos) {
                filteredIndices.push_back((int)i);
            }
        }
    }

    float visibleHeight = listAreaBottom - listAreaTop;
    float maxScroll = (filteredIndices.size() * 121.0f) - visibleHeight;
    if (maxScroll < 0.0f) maxScroll = 0.0f;
    if (*scrollOffset < 0.0f) *scrollOffset = 0.0f;
    if (*scrollOffset > maxScroll) *scrollOffset = maxScroll;

    BeginScissorMode((int)leftPanelRec.x, (int)listAreaTop, 
                      (int)leftPanelRec.width, (int)(listAreaBottom - listAreaTop));

    for (size_t j = 0; j < filteredIndices.size(); ++j) {
        int i = filteredIndices[j];
        const auto& song = songs[i];
        float rowY = 245.0f - *scrollOffset + (j * 121.0f);

        if (rowY + 101.0f > listAreaTop && rowY < listAreaBottom) {
            bool isCurrentSong = ((int)i == currentSongIndex);
            Color rowColor = isCurrentSong ? Color{255, 255, 255, 130} : Color{255, 255, 255, 76};

            // Row background
            Rectangle rowRec = { 82.0f, rowY, 569.0f, 101.0f };
            
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, rowRec)) {
                clickedIndex = (int)i;
            }
            if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && CheckCollisionPointRec(virtualMouse, rowRec)) {
                *rightClickedSongIndex = (int)i;
                *rightClickedRowRec = rowRec;
            }

            DrawRectangleRounded(rowRec, 0.15f, 8, rowColor);

            // Thumbnail
            Rectangle thumbRec = { 100.0f, rowY + 16.0f, 70.0f, 70.0f };
            Color leftColor = { 0, 31, 62, 255 };
            Color rightColor = { 0, 115, 230, 255 };
            DrawRectangleGradientEx(thumbRec, leftColor, leftColor, rightColor, rightColor);

            // Thumbnail fallback
            if (!song.title().empty()) {
                std::string fallbackStr(1, (char)toupper(song.title()[0]));
                Vector2 textSize = MeasureTextEx(poppinsFontBold, fallbackStr.c_str(), 30.0f * FONT_SCALE, 1.0f);
                Vector2 textPos = {
                    thumbRec.x + (thumbRec.width - textSize.x) / 2.0f,
                    thumbRec.y + (thumbRec.height - textSize.y) / 2.0f
                };
                DrawTextEx(poppinsFontBold, fallbackStr.c_str(), textPos, 30.0f * FONT_SCALE, 1.0f, WHITE);
            }

            // Title
            Vector2 titlePos = { 199.0f, rowY + 25.0f };
            std::string titleStr = TruncateText(poppinsFontBold, song.title(), 25.0f * FONT_SCALE, 1.0f, 400.0f);
            DrawTextEx(poppinsFontBold, titleStr.c_str(), titlePos, 25.0f * FONT_SCALE, 1.0f, WHITE);

            // Artist
            Vector2 artistPos = { 199.0f, rowY + 55.0f };
            std::string artistStr = TruncateText(poppinsFont, song.artist(), 20.0f * FONT_SCALE, 1.0f, 400.0f);
            DrawTextEx(poppinsFont, artistStr.c_str(), artistPos, 20.0f * FONT_SCALE, 1.0f, WHITE);
        }
    }

    EndScissorMode();
    return clickedIndex;
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