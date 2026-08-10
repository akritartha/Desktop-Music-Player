#include "controls.h"
#include "raylib.h"

#define FONT_SCALE 1.0f

static Texture2D shuffleTex;
static Texture2D skipTex;
static Texture2D playTex;
static Texture2D pauseTex;
static Texture2D repeatTex;
static Texture2D volumeTex;

void LoadControlIcons() {
    shuffleTex = LoadTexture("icons/shuffle.png");
    SetTextureFilter(shuffleTex, TEXTURE_FILTER_BILINEAR);
    
    skipTex = LoadTexture("icons/skip.png");
    SetTextureFilter(skipTex, TEXTURE_FILTER_BILINEAR);
    
    playTex = LoadTexture("icons/play.png");
    SetTextureFilter(playTex, TEXTURE_FILTER_BILINEAR);
    
    pauseTex = LoadTexture("icons/pause.png");
    SetTextureFilter(pauseTex, TEXTURE_FILTER_BILINEAR);
    
    repeatTex = LoadTexture("icons/repeat.png");
    SetTextureFilter(repeatTex, TEXTURE_FILTER_BILINEAR);
    
    volumeTex = LoadTexture("icons/volume.png");
    SetTextureFilter(volumeTex, TEXTURE_FILTER_BILINEAR);
}

void UnloadControlIcons() {
    UnloadTexture(shuffleTex);
    UnloadTexture(skipTex);
    UnloadTexture(playTex);
    UnloadTexture(pauseTex);
    UnloadTexture(repeatTex);
    UnloadTexture(volumeTex);
}

void DrawControls(bool isPlaying, bool isRepeatOn) {
    Vector2 origin = {0.0f, 0.0f};

    // a. Shuffle icon
    Rectangle shuffleSrc = { 0.0f, 0.0f, (float)shuffleTex.width, (float)shuffleTex.height };
    Rectangle shuffleDest = { 672.0f, 969.0f, 50.0f, 50.0f };
    DrawTexturePro(shuffleTex, shuffleSrc, shuffleDest, origin, 0.0f, WHITE);

    // b. Previous button (flipped skip icon)
    Rectangle prevSrc = { 0.0f, 0.0f, (float)skipTex.width * -1.0f, (float)skipTex.height };
    Rectangle prevDest = { 793.0f, 965.0f, 60.0f, 60.0f };
    DrawTexturePro(skipTex, prevSrc, prevDest, origin, 0.0f, WHITE);

    // c. Play/Pause button
    Texture2D activePlayPauseTex = isPlaying ? pauseTex : playTex;
    Rectangle playSrc = { 0.0f, 0.0f, (float)activePlayPauseTex.width, (float)activePlayPauseTex.height };
    Rectangle playDest = { 923.0f, 955.0f, 80.0f, 80.0f };
    DrawTexturePro(activePlayPauseTex, playSrc, playDest, origin, 0.0f, WHITE);

    // d. Next button (normal skip icon)
    Rectangle nextSrc = { 0.0f, 0.0f, (float)skipTex.width, (float)skipTex.height };
    Rectangle nextDest = { 1075.0f, 960.0f, 60.0f, 60.0f };
    DrawTexturePro(skipTex, nextSrc, nextDest, origin, 0.0f, WHITE);

    // e. Repeat icon
    Rectangle repeatSrc = { 0.0f, 0.0f, (float)repeatTex.width, (float)repeatTex.height };
    Rectangle repeatDest = { 1190.0f, 965.0f, 50.0f, 50.0f };
    Color repeatColor = isRepeatOn ? SKYBLUE : WHITE;
    DrawTexturePro(repeatTex, repeatSrc, repeatDest, origin, 0.0f, repeatColor);

    // f. Volume icon
    Rectangle volumeSrc = { 0.0f, 0.0f, (float)volumeTex.width, (float)volumeTex.height };
    Rectangle volumeDest = { 1580.0f, 974.0f, 40.0f, 40.0f };
    DrawTexturePro(volumeTex, volumeSrc, volumeDest, origin, 0.0f, WHITE);
}

bool IsPlayButtonClicked(Vector2 virtualMouse) {
    Rectangle playBtnRec = { 923.0f, 955.0f, 80.0f, 80.0f };
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, playBtnRec)) {
        return true;
    }
    return false;
}

bool IsNextButtonClicked(Vector2 virtualMouse) {
    Rectangle nextBtnRec = { 1075.0f, 960.0f, 60.0f, 60.0f };
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, nextBtnRec)) {
        return true;
    }
    return false;
}

bool IsPreviousButtonClicked(Vector2 virtualMouse) {
    Rectangle prevBtnRec = { 793.0f, 965.0f, 60.0f, 60.0f };
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, prevBtnRec)) {
        return true;
    }
    return false;
}

bool IsRepeatButtonClicked(Vector2 virtualMouse) {
    Rectangle repeatBtnRec = { 1190.0f, 965.0f, 50.0f, 50.0f };
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, repeatBtnRec)) {
        return true;
    }
    return false;
}