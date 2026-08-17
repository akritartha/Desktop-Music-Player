#include "scrollablepanel.h"

ScrollablePanel::ScrollablePanel(Rectangle b) : UIComponent(b), scrollOffset(0.0f), contentHeight(0.0f) {}

void ScrollablePanel::HandleScroll(Vector2 mousePos) {
    if (CheckCollisionPointRec(mousePos, bounds)) {
        float wheelMove = GetMouseWheelMove();
        scrollOffset -= wheelMove * 30.0f;
    }

    float visibleHeight = bounds.height;
    float maxScroll = contentHeight - visibleHeight;
    if (maxScroll < 0.0f) maxScroll = 0.0f;
    if (scrollOffset < 0.0f) scrollOffset = 0.0f;
    if (scrollOffset > maxScroll) scrollOffset = maxScroll;
}

void ScrollablePanel::BeginClip() {
    BeginScissorMode((int)bounds.x, (int)bounds.y, (int)bounds.width, (int)bounds.height);
}

void ScrollablePanel::EndClip() {
    EndScissorMode();
}

float ScrollablePanel::GetScrollOffset() const {
    return scrollOffset;
}

void ScrollablePanel::SetScrollOffset(float offset) {
    scrollOffset = offset;
}

void ScrollablePanel::SetContentHeight(float h) {
    contentHeight = h;
}
