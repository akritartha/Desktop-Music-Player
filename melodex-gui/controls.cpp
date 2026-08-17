#include "controls.h"
#include "raylib.h"

static Texture2D shuffleTex;
static Texture2D skipTex;
static Texture2D prevTex;
static Texture2D playTex;
static Texture2D pauseTex;
static Texture2D repeatTex;
static Texture2D volumeTex;
static Texture2D muteTex;

IconButton* shuffleBtn = nullptr;
IconButton* prevBtn = nullptr;
IconButton* playBtn = nullptr;
IconButton* pauseBtn = nullptr;
IconButton* nextBtn = nullptr;
IconButton* repeatBtn = nullptr;
IconButton* volumeBtn = nullptr;
IconButton* muteBtn = nullptr;

void LoadControlIcons() {
    shuffleTex = LoadTexture("melodex-gui/icons/shuffle.png");
    SetTextureFilter(shuffleTex, TEXTURE_FILTER_BILINEAR);
    
    skipTex = LoadTexture("melodex-gui/icons/skip.png");
    SetTextureFilter(skipTex, TEXTURE_FILTER_BILINEAR);
    
    Image prevImg = LoadImageFromTexture(skipTex);
    ImageFlipHorizontal(&prevImg);
    prevTex = LoadTextureFromImage(prevImg);
    UnloadImage(prevImg);
    SetTextureFilter(prevTex, TEXTURE_FILTER_BILINEAR);
    
    playTex = LoadTexture("melodex-gui/icons/play.png");
    SetTextureFilter(playTex, TEXTURE_FILTER_BILINEAR);
    
    pauseTex = LoadTexture("melodex-gui/icons/pause.png");
    SetTextureFilter(pauseTex, TEXTURE_FILTER_BILINEAR);
    
    repeatTex = LoadTexture("melodex-gui/icons/repeat.png");
    SetTextureFilter(repeatTex, TEXTURE_FILTER_BILINEAR);
    
    Image volumeImg = LoadImage("melodex-gui/icons/volume.png");
    ImageColorReplace(&volumeImg, BLACK, WHITE);
    volumeTex = LoadTextureFromImage(volumeImg);
    UnloadImage(volumeImg);
    SetTextureFilter(volumeTex, TEXTURE_FILTER_BILINEAR);

    Image muteImg = LoadImage("melodex-gui/icons/mute.png");
    ImageColorReplace(&muteImg, BLACK, WHITE);
    muteTex = LoadTextureFromImage(muteImg);
    UnloadImage(muteImg);
    SetTextureFilter(muteTex, TEXTURE_FILTER_BILINEAR);

    shuffleBtn = new IconButton({ 672.0f, 969.0f, 50.0f, 50.0f }, shuffleTex);
    prevBtn = new IconButton({ 793.0f, 965.0f, 60.0f, 60.0f }, prevTex);
    playBtn = new IconButton({ 923.0f, 955.0f, 80.0f, 80.0f }, playTex);
    pauseBtn = new IconButton({ 923.0f, 955.0f, 80.0f, 80.0f }, pauseTex);
    nextBtn = new IconButton({ 1075.0f, 960.0f, 60.0f, 60.0f }, skipTex);
    repeatBtn = new IconButton({ 1190.0f, 965.0f, 50.0f, 50.0f }, repeatTex);
    volumeBtn = new IconButton({ 1580.0f, 974.0f, 40.0f, 40.0f }, volumeTex);
    muteBtn = new IconButton({ 1580.0f, 974.0f, 40.0f, 40.0f }, muteTex);
}

void UnloadControlIcons() {
    delete shuffleBtn; shuffleBtn = nullptr;
    delete prevBtn; prevBtn = nullptr;
    delete playBtn; playBtn = nullptr;
    delete pauseBtn; pauseBtn = nullptr;
    delete nextBtn; nextBtn = nullptr;
    delete repeatBtn; repeatBtn = nullptr;
    delete volumeBtn; volumeBtn = nullptr;
    delete muteBtn; muteBtn = nullptr;

    UnloadTexture(shuffleTex);
    UnloadTexture(skipTex);
    UnloadTexture(prevTex);
    UnloadTexture(playTex);
    UnloadTexture(pauseTex);
    UnloadTexture(repeatTex);
    UnloadTexture(volumeTex); 
    UnloadTexture(muteTex); 
}

void DrawControls(bool isPlaying, bool isRepeatOn, bool isShuffleOn, bool isMuted) {
    if (shuffleBtn) {
        shuffleBtn->SetTint(isShuffleOn ? SKYBLUE : WHITE);
        shuffleBtn->Draw();
    }
    
    if (prevBtn) prevBtn->Draw();
    
    if (isPlaying) {
        if (pauseBtn) pauseBtn->Draw();
    } else {
        if (playBtn) playBtn->Draw();
    }
    
    if (nextBtn) nextBtn->Draw();
    
    if (repeatBtn) {
        repeatBtn->SetTint(isRepeatOn ? SKYBLUE : WHITE);
        repeatBtn->Draw();
    }
    
    if (isMuted) {
        if (muteBtn) muteBtn->Draw();
    } else {
        if (volumeBtn) volumeBtn->Draw();
    }
}