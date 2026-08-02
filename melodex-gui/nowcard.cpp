#include "nowcard.h"
#include "raylib.h"
#include "textutils.h"

#define FONT_SCALE 1.0f

void DrawNowCard(Font jotiOneFont, Font jotiOneFontBold, Font poppinsFont, const char* songTitle, const char* artistName) {
    // 1. Top translucent panel
    Rectangle topPanelRec = { 720.0f, 120.0f, 480.0f, 540.0f };
    Color panelColor = { 255, 255, 255, 76 };
    DrawRectangleRounded(topPanelRec, 0.08f, 8, panelColor);

    // 2. Bottom translucent panel
    Rectangle bottomPanelRec = { 720.0f, 670.0f, 480.0f, 170.0f };
    DrawRectangleRounded(bottomPanelRec, 0.2f, 8, panelColor);

    // 3. Big album art
    Rectangle albumRec = { 761.0f, 161.0f, 399.0f, 399.0f };
    Color leftColor = { 0, 31, 62, 255 };
    Color rightColor = { 0, 115, 230, 255 };
    DrawRectangleGradientEx(albumRec, leftColor, leftColor, rightColor, rightColor);

    // 4. Big "M" letter
    Vector2 mPos = { 920.0f, 300.0f };
    DrawTextEx(jotiOneFont, "M", mPos, 130.0f * FONT_SCALE, 2.0f, WHITE);

    // 5. Song title
    Vector2 titlePos = { 777.0f, 574.0f };
    std::string truncTitle = TruncateText(jotiOneFontBold, songTitle, 35.0f * FONT_SCALE, 1.0f, 350.0f);
    DrawTextEx(jotiOneFontBold, truncTitle.c_str(), titlePos, 35.0f * FONT_SCALE, 1.0f, WHITE);

    // 6. Artist name
    Vector2 artistPos = { 777.0f, 613.0f };
    std::string truncArtist = TruncateText(poppinsFont, artistName, 23.0f * FONT_SCALE, 1.0f, 350.0f);
    DrawTextEx(poppinsFont, truncArtist.c_str(), artistPos, 23.0f * FONT_SCALE, 1.0f, WHITE);

    // 7. "+ Add Lyrics" button
    Rectangle btnRec = { 853.0f, 733.0f, 203.0f, 51.0f };
    DrawRectangleRoundedLinesEx(btnRec, 0.9f, 8, 3.0f, WHITE);
    
    Vector2 btnTextPos = { 880.0f, 745.0f };
    DrawTextEx(poppinsFont, "+ Add Lyrics", btnTextPos, 27.0f * FONT_SCALE, 1.0f, WHITE);
}
