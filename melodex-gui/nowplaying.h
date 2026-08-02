#ifndef NOWPLAYING_H
#define NOWPLAYING_H

#include "raylib.h"

void DrawNowPlayingBar(Font poppinsFont, Font poppinsBold, const char* songTitle, const char* artistName, float progressPercent, const char* currentTime, const char* totalTime, float soundLevel);
float UpdateSoundLevel(Vector2 virtualMouse, float currentSoundLevel);
float UpdateSeekPosition(Vector2 virtualMouse, float currentProgressPercent, float totalSeconds);

#endif // NOWPLAYING_H
