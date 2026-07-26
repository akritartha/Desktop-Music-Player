#ifndef CONTROLS_H
#define CONTROLS_H

#include "raylib.h"

void LoadControlIcons();
void UnloadControlIcons();
void DrawControls(bool isPlaying);
bool IsPlayButtonClicked(Vector2 virtualMouse);

#endif // CONTROLS_H
