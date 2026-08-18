#include "nowplaying.h"
#include "raylib.h"
#include "textutils.h"
#include <math.h>
#include "slider.h"

#define FONT_SCALE 1.0f

static Slider seekSlider({ 505.0f, 915.0f, 910.0f, 8.7f }, 0.0f);
static Slider volumeSlider({ 1642.0f, 990.0f, 175.0f, 7.0f }, 0.0f);
static Texture2D nowPlayingThumbnail = { 0 };
static bool nowPlayingThumbnailLoaded = false;

void DrawNowPlayingBar(Font poppinsFont, Font poppinsBold, const char* songTitle, const char* artistName, float progressPercent, const char* currentTime, const char* totalTime, float soundLevel) {
    // 1. Background bar
    Rectangle bgRec = { 50.0f, 879.13f, 1810.0f, 165.87f };
    Color bgColor = { 255, 255, 255, 76 };
    DrawRectangleRounded(bgRec, 0.15f, 8, bgColor);
    
    // 2. Mini album art
    Rectangle albumRec = { 60.0f, 948.0f, 84.0f, 84.0f };
    if (!nowPlayingThumbnailLoaded) {
        if (FileExists("melodex-gui/icons/songthumbnail.jpg")) {
            nowPlayingThumbnail = LoadTexture("melodex-gui/icons/songthumbnail.jpg");
            SetTextureFilter(nowPlayingThumbnail, TEXTURE_FILTER_BILINEAR);
        }
        nowPlayingThumbnailLoaded = true;
    }

    if (nowPlayingThumbnail.id != 0) {
        Rectangle texSource = { 0.0f, 0.0f, (float)nowPlayingThumbnail.width, (float)nowPlayingThumbnail.height };
        DrawTexturePro(nowPlayingThumbnail, texSource, albumRec, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);
    } else {
        Color leftCol = { 0, 31, 62, 255 };
        Color rightCol = { 0, 115, 230, 255 };
        DrawRectangleGradientEx(albumRec, leftCol, leftCol, rightCol, rightCol);
        Vector2 mPos = { 87.0f, 966.0f };
        DrawTextEx(poppinsFont, "M", mPos, 50.0f * FONT_SCALE, 100.0f, WHITE);
    }
    
    // 3. Song title
    Vector2 titlePos = { 177.0f, 962.0f };
    std::string truncTitle = TruncateText(poppinsBold, songTitle, 30.0f * FONT_SCALE, 1.0f, 250.0f);
    DrawTextEx(poppinsBold, truncTitle.c_str(), titlePos, 30.0f * FONT_SCALE, 1.0f, WHITE);
    
    // 5. Artist name
    Vector2 artistPos = { 177.0f, 992.0f };
    std::string truncArtist = TruncateText(poppinsFont, artistName, 23.0f * FONT_SCALE, 1.0f, 250.0f);
    DrawTextEx(poppinsFont, truncArtist.c_str(), artistPos, 23.0f * FONT_SCALE, 1.0f, WHITE);

    // 6 & 7. Background progress track & Filled progress
    seekSlider = Slider({ 505.0f, 915.0f, 910.0f, 8.7f }, progressPercent);
    seekSlider.Draw();

    // 8. Current time text
    Vector2 currentTimePos = { 445.0f, 905.0f };
    DrawTextEx(poppinsFont, currentTime, currentTimePos, 30.0f * FONT_SCALE, 1.0f, WHITE);

    // 9. Total time text
    Vector2 totalTextSize = MeasureTextEx(poppinsFont, totalTime, 24.0f * FONT_SCALE, 1.0f);
    Vector2 totalTimePos = { 1475.0f - totalTextSize.x, 905.0f };
    DrawTextEx(poppinsFont, totalTime, totalTimePos, 30.0f * FONT_SCALE, 1.0f, WHITE);

    // 10 & 11. sound level background track & filled portion (current volume)
    volumeSlider = Slider({ 1642.0f, 990.0f, 175.0f, 7.0f }, soundLevel);
    volumeSlider.Draw();
}

float UpdateSoundLevel(Vector2 virtualMouse, float currentSoundLevel) {
    volumeSlider = Slider({ 1642.0f, 990.0f, 175.0f, 7.0f }, currentSoundLevel);
    return volumeSlider.UpdateDrag(virtualMouse);
}

float UpdateSeekPosition(Vector2 virtualMouse, float currentProgressPercent, float totalSeconds) {
    seekSlider = Slider({ 505.0f, 915.0f, 910.0f, 8.7f }, currentProgressPercent);
    return seekSlider.UpdateDrag(virtualMouse);
}
