#ifndef UICOMPONENT_H
#define UICOMPONENT_H

#include "raylib.h"

class UIComponent {
protected:
    Rectangle bounds;
public:
    UIComponent(Rectangle b) : bounds(b) {}
    virtual bool IsClicked(Vector2 mousePos) const {
        return IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePos, bounds);
    }
    virtual void Draw() = 0;
    virtual ~UIComponent() = default;
};

#endif // UICOMPONENT_H
