#include "topbar.h"
#include "raylib.h"

#define FONT_SCALE 1.0f

static Texture2D logoTexture;

void LoadTopBarAssets() {
    logoTexture = LoadTexture("logo.png");
    SetTextureFilter(logoTexture, TEXTURE_FILTER_BILINEAR);
}

void UnloadTopBarAssets() {
    UnloadTexture(logoTexture);
}

void DrawTopBar(Font jotiOneFont) {
    Rectangle logoSource = { 0, 0, (float)logoTexture.width, (float)logoTexture.height };
    Rectangle logoDest = { 50.0f, 20.0f, 60.0f, 70.0f };
    DrawTexturePro(logoTexture, logoSource, logoDest, (Vector2){0, 0}, 0.0f, WHITE);
    
    Vector2 textPos = { 120.0f, 31.0f };
    DrawTextEx(jotiOneFont, "Melodex", textPos, 45.0f * FONT_SCALE, 1.5f, WHITE);
}
