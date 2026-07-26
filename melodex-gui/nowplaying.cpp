#include "nowplaying.h"
#include "raylib.h"

#define FONT_SCALE 1.0f

void DrawNowPlayingBar(Font poppinsFont, Font poppinsBold, const char* songTitle, const char* artistName, float progressPercent, const char* currentTime, const char* totalTime, float soundLevel) {
    // 1. Background bar
    Rectangle bgRec = { 50.0f, 879.13f, 1810.0f, 165.87f };
    Color bgColor = { 255, 255, 255, 76 };
    DrawRectangleRounded(bgRec, 0.15f, 8, bgColor);
    
    // 2. Mini album art using DrawRectangleGradientEx
    Rectangle albumRec = { 60.0f, 948.0f, 84.0f, 84.0f };
    Color leftCol = { 0, 31, 62, 255 };
    Color rightCol = { 0, 115, 230, 255 };
    // col1: top-left, col2: bottom-left, col3: bottom-right, col4: top-right
    DrawRectangleGradientEx(albumRec, leftCol, leftCol, rightCol, rightCol);
    
    // 3. "M" letter
    Vector2 mPos = { 87.0f, 966.0f };
    DrawTextEx(poppinsFont, "M", mPos, 50.0f * FONT_SCALE, 100.0f, WHITE);
    
    // 4. Song title
    Vector2 titlePos = { 177.0f, 962.0f };
    DrawTextEx(poppinsBold, songTitle, titlePos, 30.0f * FONT_SCALE, 1.0f, WHITE);
    
    // 5. Artist name
    Vector2 artistPos = { 177.0f, 992.0f };
    DrawTextEx(poppinsFont, artistName, artistPos, 23.0f * FONT_SCALE, 1.0f, WHITE);

    // 6. Background progress track
    Rectangle bgProgressRec = { 505.0f, 915.0f, 910.0f, 8.7f };
    Color bgProgressColor = { 255, 255, 255, 128 };
    DrawRectangleRounded(bgProgressRec, 0.8f, 6, bgProgressColor);

    // 7. Filled progress
    Rectangle filledProgressRec = { 505.0f, 915.0f, 910.0f * progressPercent, 8.7f };
    DrawRectangleRounded(filledProgressRec, 0.8f, 6, WHITE);

    // 8. Current time text
    Vector2 currentTimePos = { 445.0f, 905.0f };
    DrawTextEx(poppinsFont, currentTime, currentTimePos, 30.0f * FONT_SCALE, 1.0f, WHITE);

    // 9. Total time text
    Vector2 totalTextSize = MeasureTextEx(poppinsFont, totalTime, 24.0f * FONT_SCALE, 1.0f);
    Vector2 totalTimePos = { 1475.0f - totalTextSize.x, 905.0f };
    DrawTextEx(poppinsFont, totalTime, totalTimePos, 30.0f * FONT_SCALE, 1.0f, WHITE);

    // 10. current sound level
    Rectangle currentSoundRec = { 1642.0f, 990.0f, 175.0f, 7.0f };
    Color currentSoundColor = { 255, 255, 255, 128 };
    DrawRectangleRounded(currentSoundRec, 0.8f, 6, currentSoundColor);

    // 11. total sound level
    Rectangle filledSoundRec = { 1642.0f, 990.0f, 175.0f * soundLevel, 7.0f };
    DrawRectangleRounded(filledSoundRec, 0.8f, 6, WHITE);
}
