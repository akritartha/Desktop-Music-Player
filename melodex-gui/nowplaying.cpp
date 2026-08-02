#include "nowplaying.h"
#include "raylib.h"
#include "textutils.h"
#include <math.h>

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
    std::string truncTitle = TruncateText(poppinsBold, songTitle, 30.0f * FONT_SCALE, 1.0f, 250.0f);
    DrawTextEx(poppinsBold, truncTitle.c_str(), titlePos, 30.0f * FONT_SCALE, 1.0f, WHITE);
    
    // 5. Artist name
    Vector2 artistPos = { 177.0f, 992.0f };
    std::string truncArtist = TruncateText(poppinsFont, artistName, 23.0f * FONT_SCALE, 1.0f, 250.0f);
    DrawTextEx(poppinsFont, truncArtist.c_str(), artistPos, 23.0f * FONT_SCALE, 1.0f, WHITE);

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

    // 10. sound level background track
    Rectangle soundTrackRec = { 1642.0f, 990.0f, 175.0f, 7.0f };
    Color soundTrackColor = { 255, 255, 255, 128 };
    DrawRectangleRounded(soundTrackRec, 0.8f, 6, soundTrackColor);

    // 11. sound level filled portion (current volume)
    Rectangle soundFillRec = { 1642.0f, 990.0f, 175.0f * soundLevel, 7.0f };
    DrawRectangleRounded(soundFillRec, 0.8f, 6, WHITE);
}

float UpdateSoundLevel(Vector2 virtualMouse, float currentSoundLevel) {
    Rectangle soundBarRec = { 1642.0f, 990.0f, 175.0f, 7.0f };
    
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && 
        virtualMouse.y >= (soundBarRec.y - 15.0f) && virtualMouse.y <= (soundBarRec.y + 15.0f) &&
        virtualMouse.x >= (soundBarRec.x - 10.0f) && virtualMouse.x <= (soundBarRec.x + soundBarRec.width + 10.0f)) {
        
        float newLevel = (virtualMouse.x - soundBarRec.x) / soundBarRec.width;
        newLevel = fmaxf(0.0f, fminf(1.0f, newLevel));
        return newLevel;
    }
    
    return currentSoundLevel;
}

float UpdateSeekPosition(Vector2 virtualMouse, float currentProgressPercent, float totalSeconds) {
    Rectangle bar = { 505.0f, 915.0f, 910.0f, 8.7f };

    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
        virtualMouse.y >= (bar.y - 15.0f) && virtualMouse.y <= (bar.y + 15.0f) &&
        virtualMouse.x >= (bar.x - 10.0f) && virtualMouse.x <= (bar.x + bar.width + 10.0f)) {

        float newPercent = (virtualMouse.x - bar.x) / bar.width;
        newPercent = fmaxf(0.0f, fminf(1.0f, newPercent));
        return newPercent;
    }

    return currentProgressPercent;
}
