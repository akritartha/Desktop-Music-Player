#ifndef BUTTON_H
#define BUTTON_H

#include "uicomponent.h"

class IconButton : public UIComponent {
private:
    Texture2D icon;
    Color tint;
public:
    IconButton(Rectangle b, Texture2D tex);
    void Draw() override;
    void SetTint(Color c);
};

#endif // BUTTON_H
