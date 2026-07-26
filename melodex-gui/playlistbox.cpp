#include "playlistbox.h"
#include "raylib.h"
#include "textutils.h"

#define FONT_SCALE 1.0f

void DrawPlaylistBox(Font poppinsFontBold) {
    Color bgPanelColor = { 255, 255, 255, 76 };

    // 1. Right box background panel
    Rectangle rightPanelRec = { 1230.0f, 120.0f, 630.0f, 720.0f };
    DrawRectangleRounded(rightPanelRec, 0.08f, 8, bgPanelColor);

    // 2. "Playlist" heading text
    Vector2 headingPos = { 1280.0f, 140.0f };
    DrawTextEx(poppinsFontBold, "Playlist", headingPos, 55.0f * FONT_SCALE, 1.5f, WHITE);
    Vector2 btnTextPos = { 1630.0f, 160.0f };
    DrawTextEx(poppinsFontBold, "+ Add Playlist", btnTextPos, 28.0f * FONT_SCALE, 1.0f, WHITE);

    // 3. Card 1 (English)
    Rectangle card1Rec = { 1284.0f, 230.0f, 211.0f, 250.0f };
    DrawRectangleRounded(card1Rec, 0.15f, 8, bgPanelColor);
    Rectangle thumb1Rec = { 1302.0f, 249.0f, 175.0f, 168.0f };
    DrawRectangleRounded(thumb1Rec, 0.12f, 8, bgPanelColor);
    Vector2 label1Pos = { 1315.0f, 428.0f };
    std::string label1 = TruncateText(poppinsFontBold, "English", 30.0f * FONT_SCALE, 1.0f, 160.0f);
    DrawTextEx(poppinsFontBold, label1.c_str(), label1Pos, 30.0f * FONT_SCALE, 1.0f, WHITE);

    // 4. Card 2 (Hindi)
    Rectangle card2Rec = { 1584.0f, 230.0f, 211.0f, 250.0f };
    DrawRectangleRounded(card2Rec, 0.15f, 8, bgPanelColor);
    Rectangle thumb2Rec = { 1602.0f, 249.0f, 175.0f, 168.0f };
    DrawRectangleRounded(thumb2Rec, 0.12f, 8, bgPanelColor);
    Vector2 label2Pos = { 1616.0f, 428.0f };
    std::string label2 = TruncateText(poppinsFontBold, "Hindi", 30.0f * FONT_SCALE, 1.0f, 160.0f);
    DrawTextEx(poppinsFontBold, label2.c_str(), label2Pos, 30.0f * FONT_SCALE, 1.0f, WHITE);

    // 5. Card 3 (Nepali)
    Rectangle card3Rec = { 1284.0f, 548.0f, 211.0f, 250.0f };
    DrawRectangleRounded(card3Rec, 0.15f, 8, bgPanelColor);
    Rectangle thumb3Rec = { 1302.0f, 567.0f, 175.0f, 168.0f };
    DrawRectangleRounded(thumb3Rec, 0.12f, 8, bgPanelColor);
    Vector2 label3Pos = { 1316.0f, 746.0f };
    std::string label3 = TruncateText(poppinsFontBold, "Nepali", 30.0f * FONT_SCALE, 1.0f, 160.0f);
    DrawTextEx(poppinsFontBold, label3.c_str(), label3Pos, 30.0f * FONT_SCALE, 1.0f, WHITE);

    // 6. Card 4 (Spanish)
    Rectangle card4Rec = { 1584.0f, 548.0f, 211.0f, 250.0f };
    DrawRectangleRounded(card4Rec, 0.15f, 8, bgPanelColor);
    Rectangle thumb4Rec = { 1602.0f, 567.0f, 175.0f, 168.0f };
    DrawRectangleRounded(thumb4Rec, 0.12f, 8, bgPanelColor);
    Vector2 label4Pos = { 1616.0f, 746.0f };
    std::string label4 = TruncateText(poppinsFontBold, "Spanish", 30.0f * FONT_SCALE, 1.0f, 160.0f);
    DrawTextEx(poppinsFontBold, label4.c_str(), label4Pos, 30.0f * FONT_SCALE, 1.0f, WHITE);
}
