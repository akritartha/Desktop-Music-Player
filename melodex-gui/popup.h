#ifndef POPUP_H
#define POPUP_H

#include "uicomponent.h"
#include "raylib.h"

class Popup : public UIComponent {
protected:
    bool isOpen;
public:
    Popup(Rectangle b);
    virtual void Open();
    virtual void Close();
    bool IsOpen() const;
    void DrawOverlay();
    virtual void Draw() override = 0;
};

#endif // POPUP_H
