#include "playlistbox.h"
#include "raylib.h"
#include "textutils.h"
#include <cmath>

#define FONT_SCALE 1.0f

int DrawPlaylistBox(Font poppinsFontBold, Vector2 virtualMouse, float* scrollOffset, const std::vector<PlaylistEntry>& playlists) {
    Color bgPanelColor = { 255, 255, 255, 76 };

    // 1. Right box background panel
    Rectangle rightPanelRec = { 1230.0f, 120.0f, 630.0f, 720.0f };
    DrawRectangleRounded(rightPanelRec, 0.08f, 8, bgPanelColor);

    // 2. "Playlist" heading text
    Vector2 headingPos = { 1280.0f, 140.0f };
    DrawTextEx(poppinsFontBold, "Playlist", headingPos, 55.0f * FONT_SCALE, 1.5f, WHITE);
    Vector2 btnTextPos = { 1630.0f, 160.0f };
    DrawTextEx(poppinsFontBold, "+ Add Playlist", btnTextPos, 28.0f * FONT_SCALE, 1.0f, WHITE);

    float listAreaTop = 230.0f;
    float listAreaBottom = rightPanelRec.y + rightPanelRec.height;
    bool mouseOverPanel = CheckCollisionPointRec(virtualMouse, rightPanelRec);

    if (mouseOverPanel) {
        float wheelMove = GetMouseWheelMove();
        *scrollOffset -= wheelMove * 30.0f;
    }

    int columns = 2;
    int rows = (int)((playlists.size() + columns - 1) / columns);
    float rowHeight = 318.0f;
    float visibleHeight = listAreaBottom - listAreaTop;
    float maxScroll = (rows * rowHeight) - visibleHeight;
    if (maxScroll < 0.0f) maxScroll = 0.0f;
    if (*scrollOffset < 0.0f) *scrollOffset = 0.0f;
    if (*scrollOffset > maxScroll) *scrollOffset = maxScroll;

    int clickedIndex = -1;

    BeginScissorMode((int)rightPanelRec.x, (int)listAreaTop, 
                      (int)rightPanelRec.width, (int)(listAreaBottom - listAreaTop));

    for (size_t i = 0; i < playlists.size(); ++i) {
        int col = (int)(i % columns);
        int row = (int)(i / columns);
        
        float cardX = 1284.0f + (col * 300.0f);
        float cardY = listAreaTop - *scrollOffset + (row * rowHeight);
        
        if (cardY + 250.0f < listAreaTop || cardY > listAreaBottom) continue;
        
        Rectangle cardRec = { cardX, cardY, 211.0f, 250.0f };
        
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, cardRec)) {
            clickedIndex = (int)i;
        }
        
        DrawRectangleRounded(cardRec, 0.15f, 8, bgPanelColor);
        
        Rectangle thumbRec = { cardX + 18.0f, cardY + 19.0f, 175.0f, 168.0f };
        DrawRectangleRounded(thumbRec, 0.12f, 8, bgPanelColor);
        
        Vector2 labelPos = { cardX + 32.0f, cardY + 198.0f };
        std::string label = TruncateText(poppinsFontBold, playlists[i].name, 30.0f * FONT_SCALE, 1.0f, 160.0f);
        DrawTextEx(poppinsFontBold, label.c_str(), labelPos, 30.0f * FONT_SCALE, 1.0f, WHITE);
    }

    EndScissorMode();

    return clickedIndex;
}
