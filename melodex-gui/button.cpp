#include "button.h"

IconButton::IconButton(Rectangle b, Texture2D tex) 
    : UIComponent(b), icon(tex), tint(WHITE) {}

void IconButton::Draw() {
    Rectangle src = { 0.0f, 0.0f, (float)icon.width, (float)icon.height };
    Vector2 origin = { 0.0f, 0.0f };
    DrawTexturePro(icon, src, bounds, origin, 0.0f, tint);
}

void IconButton::SetTint(Color c) {
    tint = c;
}
