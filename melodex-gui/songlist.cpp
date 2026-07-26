#include "songlist.h"
#include "raylib.h"

#define FONT_SCALE 1.0f

static Texture2D magnifierTexture;

void LoadSongListAssets() {
    magnifierTexture = LoadTexture("icons/magnifier.png");
    SetTextureFilter(magnifierTexture, TEXTURE_FILTER_BILINEAR);
}

void UnloadSongListAssets() {
    UnloadTexture(magnifierTexture);
}

void DrawSongList(Font poppinsFont, Font poppinsFontBold) {
    Color bgPanelColor = { 255, 255, 255, 76 };

    // 1. Left box background panel
    Rectangle leftPanelRec = { 50.0f, 120.0f, 639.0f, 720.0f };
    DrawRectangleRounded(leftPanelRec, 0.08f, 8, bgPanelColor);

    // 2. Search bar background
    Rectangle searchRec = { 82.0f, 148.0f, 569.0f, 60.0f };
    DrawRectangleRounded(searchRec, 0.25f, 8, bgPanelColor);

    // 3. Search placeholder text
    Vector2 searchPos = { 150.0f, 165.0f };
    Color placeholderColor = { 255, 255, 255, 220 };
    DrawTextEx(poppinsFont, "Search for Songs", searchPos, 25.0f * FONT_SCALE, 1.0f, placeholderColor);

    Rectangle magSource = { 0, 0, (float)magnifierTexture.width, (float)magnifierTexture.height };
    Rectangle magDest = { 98.0f, 163.0f, 30.0f, 30.0f };
    DrawTexturePro(magnifierTexture, magSource, magDest, (Vector2){0, 0}, 0.0f, WHITE);

    // 4. First song row background
    Rectangle row1Rec = { 82.0f, 245.0f, 569.0f, 101.0f };
    DrawRectangleRounded(row1Rec, 0.15f, 8, bgPanelColor);

    // 5. First song row album thumbnail
    Rectangle thumb1Rec = { 100.0f, 261.0f, 70.0f, 70.0f };
    DrawRectangleRounded(thumb1Rec, 0.14f, 8, bgPanelColor);

    // 6. First song title
    Vector2 title1Pos = { 199.0f, 270.0f };
    DrawTextEx(poppinsFontBold, "Hey There Delilah", title1Pos, 25.0f * FONT_SCALE, 1.0f, WHITE);

    // 7. First song artist
    Vector2 artist1Pos = { 199.0f, 300.0f };
    DrawTextEx(poppinsFont, "Plain White T's", artist1Pos, 20.0f * FONT_SCALE, 1.0f, WHITE);

    // 8. Second song row background
    Rectangle row2Rec = { 82.0f, 366.0f, 569.0f, 101.0f };
    DrawRectangleRounded(row2Rec, 0.15f, 8, bgPanelColor);

    // 9. Second song row album thumbnail
    Rectangle thumb2Rec = { 100.0f, 382.0f, 70.0f, 70.0f };
    DrawRectangleRounded(thumb2Rec, 0.14f, 8, bgPanelColor);

    // 10. Second song title
    Vector2 title2Pos = { 199.0f, 390.0f };
    DrawTextEx(poppinsFontBold, "Blue", title2Pos, 25.0f * FONT_SCALE, 1.0f, WHITE);

    // 11. Second song artist
    Vector2 artist2Pos = { 199.0f, 420.0f };
    DrawTextEx(poppinsFont, "Yung Kai", artist2Pos, 20.0f * FONT_SCALE, 1.0f, WHITE);
}
