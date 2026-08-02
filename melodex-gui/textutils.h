#ifndef TEXTUTILS_H
#define TEXTUTILS_H

#include "raylib.h"
#include <string>

std::string TruncateText(Font font, const std::string& text, float fontSize, float spacing, float maxWidth);
std::string FormatTime(float totalSeconds);

#endif // TEXTUTILS_H
