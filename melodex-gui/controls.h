#ifndef CONTROLS_H
#define CONTROLS_H

#include "raylib.h"

void LoadControlIcons();
void UnloadControlIcons();
void DrawControls(bool isPlaying, bool isRepeatOn, bool isShuffleOn,bool isMuted);
bool IsPlayButtonClicked(Vector2 virtualMouse);
bool IsNextButtonClicked(Vector2 virtualMouse);
bool IsPreviousButtonClicked(Vector2 virtualMouse);
bool IsRepeatButtonClicked(Vector2 virtualMouse);
bool IsShuffleButtonClicked(Vector2 virtualMouse);
bool IsVolumeButtonClicked(Vector2 virtualMouse);

#endif // CONTROLS_H
