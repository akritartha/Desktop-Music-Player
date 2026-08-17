#ifndef SCROLLABLEPANEL_H
#define SCROLLABLEPANEL_H

#include "uicomponent.h"
#include "raylib.h"

class ScrollablePanel : public UIComponent {
protected:
    float scrollOffset;
    float contentHeight;
public:
    ScrollablePanel(Rectangle b);
    void HandleScroll(Vector2 mousePos);
    void BeginClip();
    void EndClip();
    float GetScrollOffset() const;
    void SetScrollOffset(float offset);
    void SetContentHeight(float h);
    virtual void Draw() override = 0;
};

#endif // SCROLLABLEPANEL_H
