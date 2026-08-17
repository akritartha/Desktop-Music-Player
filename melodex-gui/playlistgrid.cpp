#include "playlistgrid.h"
#include "textutils.h"

#define FONT_SCALE 1.0f

PlaylistGrid::PlaylistGrid(Rectangle b, Font poppinsBold, Vector2 mouse, 
                           std::vector<PlaylistEntry>& p, int* rClickIdx)
    : ScrollablePanel(b), poppinsFontBold(poppinsBold), virtualMouse(mouse),
      playlists(p), rightClickedPlaylistIndex(rClickIdx), clickedIndex(-1) {}

void PlaylistGrid::Draw() {
    int columns = 2;
    int rows = (int)((playlists.size() + columns - 1) / columns);
    float rowHeight = 318.0f;
    
    SetContentHeight(rows * rowHeight);
    HandleScroll(virtualMouse);
    BeginClip();

    Color bgPanelColor = { 255, 255, 255, 76 };

    for (size_t i = 0; i < playlists.size(); ++i) {
        int col = (int)(i % columns);
        int row = (int)(i / columns);
        
        float cardX = 1284.0f + (col * 300.0f);
        float cardY = bounds.y - scrollOffset + (row * rowHeight);
        
        if (cardY + 250.0f < bounds.y || cardY > bounds.y + bounds.height) continue;
        
        Rectangle cardRec = { cardX, cardY, 211.0f, 250.0f };
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, cardRec)) {
            clickedIndex = (int)i;
        }
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && CheckCollisionPointRec(virtualMouse, cardRec) && !playlists[i].isDefault()) {
            *rightClickedPlaylistIndex = (int)i;
        }
        DrawRectangleRounded(cardRec, 0.15f, 8, bgPanelColor);
        
        Rectangle thumbRec = { cardX + 18.0f, cardY + 19.0f, 175.0f, 168.0f };
        
        PlaylistEntry& playlist = playlists[i];
        if (!playlist.thumbnailPath().empty() && !playlist.textureLoaded()) {
            if (FileExists(playlist.thumbnailPath().c_str())) {
                playlist.setThumbnailTexture(LoadTexture(playlist.thumbnailPath().c_str()));
                SetTextureFilter(playlist.thumbnailTexture(), TEXTURE_FILTER_BILINEAR);
            }
            playlist.setTextureLoaded(true);
        }

        if (playlist.textureLoaded() && playlist.thumbnailTexture().id != 0) {
            Rectangle texSource = { 0, 0, (float)playlist.thumbnailTexture().width, (float)playlist.thumbnailTexture().height };
            DrawTexturePro(playlist.thumbnailTexture(), texSource, thumbRec, (Vector2){0, 0}, 0.0f, WHITE);
        } else {
            DrawRectangleRounded(thumbRec, 0.12f, 8, bgPanelColor);
        }
        
        Vector2 labelPos = { cardX + 32.0f, cardY + 198.0f };
        std::string label = TruncateText(poppinsFontBold, playlists[i].name(), 30.0f * FONT_SCALE, 1.0f, 160.0f);
        DrawTextEx(poppinsFontBold, label.c_str(), labelPos, 30.0f * FONT_SCALE, 1.0f, WHITE);
    }

    EndClip();
}

int PlaylistGrid::GetClickedIndex() const {
    return clickedIndex;
}
