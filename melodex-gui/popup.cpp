#include "popup.h"

Popup::Popup(Rectangle b) : UIComponent(b), isOpen(false) {}

void Popup::Open() {
    isOpen = true;
}

void Popup::Close() {
    isOpen = false;
}

bool Popup::IsOpen() const {
    return isOpen;
}

void Popup::DrawOverlay() {
    DrawRectangle(0, 0, 1920, 1080, {0, 0, 0, 150});
}
