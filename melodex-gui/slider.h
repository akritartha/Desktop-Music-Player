#ifndef SLIDER_H
#define SLIDER_H

#include "uicomponent.h"

class Slider : public UIComponent {
private:
    float percent;
public:
    Slider(Rectangle b, float initialPercent);
    void Draw() override;
    float UpdateDrag(Vector2 mousePos);
    float GetPercent() const;
};

#endif // SLIDER_H
