#include "nowcard.h"
#include "raylib.h"
#include "textutils.h"

#define FONT_SCALE 1.0f

void DrawNowCard(Font jotiOneFont, Font jotiOneFontBold, Font poppinsFont, const char *songTitle, const char *artistName, bool Isplaying)
{
    // 1. Top translucent panel
    Rectangle topPanelRec = {720.0f, 120.0f, 480.0f, 600.0f};
    Color panelColor = {255, 255, 255, 76};
    DrawRectangleRounded(topPanelRec, 0.08f, 8, panelColor);

    // 2. Bottom translucent panel
    Rectangle bottomPanelRec = {720.0f, 730.0f, 480.0f, 110.0f};
    DrawRectangleRounded(bottomPanelRec, 0.2f, 8, panelColor);

    // 3. Big album art
    Rectangle albumRec = {761.0f, 170.0f, 399.0f, 399.0f};
    Color leftColor = {0, 31, 62, 255};
    Color rightColor = {0, 115, 230, 255};
    DrawRectangleGradientEx(albumRec, leftColor, leftColor, rightColor, rightColor);

    // 4. Big "M" letter
    Vector2 mPos = {920.0f, 307.0f};
    DrawTextEx(jotiOneFont, "M", mPos, 130.0f * FONT_SCALE, 2.0f, WHITE);
    
    // static Texture2D logoTexture;
    // logoTexture = LoadTexture("melodex-gui/icons/songthumbnail.jpg");
    // SetTextureFilter(logoTexture, TEXTURE_FILTER_BILINEAR);
    // Rectangle logoSource = { 0, 0, (float)logoTexture.width, (float)logoTexture.height };
    // Rectangle logoDest = { 500.0f, 500.0f, 60.0f, 70.0f };
    // DrawTexturePro(logoTexture,logoSource,logoDest,(Vector2){0, 0},0.0f,WHITE);
    // UnloadTexture(logoTexture);

    // 5. Song title
    Vector2 titlePos = {777.0f, 604.0f};
    std::string truncTitle = TruncateText(jotiOneFontBold, songTitle, 35.0f * FONT_SCALE, 1.0f, 350.0f);
    DrawTextEx(jotiOneFontBold, truncTitle.c_str(), titlePos, 35.0f * FONT_SCALE, 1.0f, WHITE);

    // 6. Artist name
    Vector2 artistPos = {777.0f, 643.0f};
    std::string truncArtist = TruncateText(poppinsFont, artistName, 25.0f * FONT_SCALE, 1.0f, 350.0f);
    DrawTextEx(poppinsFont, truncArtist.c_str(), artistPos, 25.0f * FONT_SCALE, 1.0f, WHITE);

    // 7. "Playing Paused Status"
    Rectangle btnRec = {853.0f, 733.0f, 203.0f, 51.0f};
    Vector2 btnTextPos = {880.0f, 760.0f};
    DrawTextEx(poppinsFont, (Isplaying) ? "Playing" : "Paused", btnTextPos, 50.0f * FONT_SCALE, 1.0f, WHITE);
}
