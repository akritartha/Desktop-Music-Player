#include "slider.h"
#include <math.h>

Slider::Slider(Rectangle b, float initialPercent) : UIComponent(b), percent(initialPercent) {}

void Slider::Draw() {
    Color trackColor = { 255, 255, 255, 128 };
    DrawRectangleRounded(bounds, 0.8f, 6, trackColor);

    Rectangle filledRec = bounds;
    filledRec.width = bounds.width * percent;
    DrawRectangleRounded(filledRec, 0.8f, 6, WHITE);
}

float Slider::UpdateDrag(Vector2 mousePos) {
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) &&
        mousePos.y >= (bounds.y - 15.0f) && mousePos.y <= (bounds.y + 15.0f) &&
        mousePos.x >= (bounds.x - 10.0f) && mousePos.x <= (bounds.x + bounds.width + 10.0f)) {
        
        float newPercent = (mousePos.x - bounds.x) / bounds.width;
        percent = fmaxf(0.0f, fminf(1.0f, newPercent));
    }
    return percent;
}

float Slider::GetPercent() const {
    return percent;
}
