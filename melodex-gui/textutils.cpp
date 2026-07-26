#include "textutils.h"

std::string TruncateText(Font font, const std::string& text, float fontSize, float spacing, float maxWidth) {
    Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, spacing);
    
    if (textSize.x <= maxWidth) {
        return text;
    }
    
    for (int i = text.length() - 1; i > 0; --i) {
        std::string candidate = text.substr(0, i) + "...";
        Vector2 candidateSize = MeasureTextEx(font, candidate.c_str(), fontSize, spacing);
        
        if (candidateSize.x <= maxWidth) {
            return candidate;
        }
    }
    
    return "...";
}
