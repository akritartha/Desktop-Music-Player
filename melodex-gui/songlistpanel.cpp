#include "songlistpanel.h"
#include "textutils.h"
#include <cctype>
#include <unordered_map>

#define FONT_SCALE 1.0f

SongListPanel::SongListPanel(Rectangle b, Font poppins, Font poppinsBold, Vector2 mouse, 
                             const std::vector<SongEntry>& s, int curIdx, const std::string& search, 
                             const std::vector<int>& activeInd, bool showAll, 
                             int* rClickIdx, Rectangle* rClickRec)
    : ScrollablePanel(b), poppinsFont(poppins), poppinsFontBold(poppinsBold), virtualMouse(mouse),
      songs(s), currentSongIndex(curIdx), searchText(search), activePlaylistSongIndices(activeInd),
      showAllSongs(showAll), rightClickedSongIndex(rClickIdx), rightClickedRowRec(rClickRec), clickedIndex(-1) {}

void SongListPanel::Draw() {
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

    SetContentHeight(filteredIndices.size() * 121.0f);
    HandleScroll(virtualMouse);
    BeginClip();

    for (size_t j = 0; j < filteredIndices.size(); ++j) {
        int i = filteredIndices[j];
        const auto& song = songs[i];
        float rowY = bounds.y - scrollOffset + (j * 121.0f);

        if (rowY + 101.0f > bounds.y && rowY < bounds.y + bounds.height) {
            bool isCurrentSong = ((int)i == currentSongIndex);
            Color rowColor = isCurrentSong ? Color{255, 255, 255, 130} : Color{255, 255, 255, 76};

            Rectangle rowRec = { 82.0f, rowY, 569.0f, 101.0f };
            
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, rowRec)) {
                clickedIndex = (int)i;
            }
            if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && CheckCollisionPointRec(virtualMouse, rowRec)) {
                *rightClickedSongIndex = (int)i;
                *rightClickedRowRec = rowRec;
            }

            DrawRectangleRounded(rowRec, 0.15f, 8, rowColor);

            Rectangle thumbRec = { 100.0f, rowY + 16.0f, 70.0f, 70.0f };

            static std::unordered_map<std::string, Texture2D> thumbnailCache;
            if (!song.thumbnailPath().empty() && FileExists(song.thumbnailPath().c_str())) {
                Texture2D& thumbnail = thumbnailCache[song.thumbnailPath()];
                if (thumbnail.id == 0) {
                    thumbnail = LoadTexture(song.thumbnailPath().c_str());
                    SetTextureFilter(thumbnail, TEXTURE_FILTER_BILINEAR);
                }

                Rectangle texSource = { 0.0f, 0.0f, (float)thumbnail.width, (float)thumbnail.height };
                DrawTexturePro(thumbnail, texSource, thumbRec, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
            } else {
                DrawRectangleRounded(thumbRec, 0.12f, 8, Color{255, 255, 255, 76});
            }

            Vector2 titlePos = { 199.0f, rowY + 25.0f };
            std::string titleStr = TruncateText(poppinsFontBold, song.title(), 25.0f * FONT_SCALE, 1.0f, 400.0f);
            DrawTextEx(poppinsFontBold, titleStr.c_str(), titlePos, 25.0f * FONT_SCALE, 1.0f, WHITE);

            Vector2 artistPos = { 199.0f, rowY + 55.0f };
            std::string artistStr = TruncateText(poppinsFont, song.artist(), 20.0f * FONT_SCALE, 1.0f, 400.0f);
            DrawTextEx(poppinsFont, artistStr.c_str(), artistPos, 20.0f * FONT_SCALE, 1.0f, WHITE);
        }
    }

    EndClip();
}

int SongListPanel::GetClickedIndex() const {
    return clickedIndex;
}
