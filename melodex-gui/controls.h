#ifndef CONTROLS_H
#define CONTROLS_H

#include "raylib.h"
#include "button.h"

extern IconButton* shuffleBtn;
extern IconButton* prevBtn;
extern IconButton* playBtn;
extern IconButton* pauseBtn;
extern IconButton* nextBtn;
extern IconButton* repeatBtn;
extern IconButton* volumeBtn;
extern IconButton* muteBtn;

void LoadControlIcons();
void UnloadControlIcons();
void DrawControls(bool isPlaying, bool isRepeatOn, bool isShuffleOn, bool isMuted);

#endif // CONTROLS_H
