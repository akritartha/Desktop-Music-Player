#include "raylib.h"
#include <math.h>
#include "topbar.h"
#include "nowplaying.h"
#include "controls.h"
#include "nowcard.h"
#include "songlist.h"
#include "playlistbox.h"

int main() {
    InitWindow(1920, 1080, "Melodex");
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(60);

    RenderTexture2D targetTexture = LoadRenderTexture(1920, 1080);
    SetTextureFilter(targetTexture.texture, TEXTURE_FILTER_BILINEAR);

    Font jotiOne = LoadFontEx("JotiOne-Regular.ttf", 64, 0, 0);
    SetTextureFilter(jotiOne.texture, TEXTURE_FILTER_BILINEAR);
    GenTextureMipmaps(&jotiOne.texture);
    Font poppins = LoadFontEx("Poppins/Poppins-Regular.ttf", 64, 0, 0);
    SetTextureFilter(poppins.texture, TEXTURE_FILTER_BILINEAR);
    GenTextureMipmaps(&poppins.texture);

    Font poppinsBold = LoadFontEx("Poppins/Poppins-Bold.ttf", 64, 0, 0);
    SetTextureFilter(poppinsBold.texture, TEXTURE_FILTER_BILINEAR);
    GenTextureMipmaps(&poppinsBold.texture);

    LoadControlIcons();
    LoadTopBarAssets();
    LoadSongListAssets();

    bool isPlaying = false;

    while (!WindowShouldClose()) {
        float scale = fminf((float)GetScreenWidth() / 1920.0f, 
                             (float)GetScreenHeight() / 1080.0f);
        
        Rectangle destRec = { 
            (GetScreenWidth() - (1920.0f * scale)) * 0.5f,
            (GetScreenHeight() - (1080.0f * scale)) * 0.5f,
            1920.0f * scale, 
            1080.0f * scale 
        };
        
        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse;
        virtualMouse.x = (mouse.x - destRec.x) / scale;
        virtualMouse.y = (mouse.y - destRec.y) / scale;

        BeginTextureMode(targetTexture);
        ClearBackground(BLACK);

        DrawRectangleGradientEx(
            (Rectangle){0, 0, 1920, 1080},
            (Color){0, 31, 62, 255},
            (Color){0, 31, 62, 255},
            (Color){0, 115, 230, 255},
            (Color){0, 115, 230, 255}
        );

        DrawTopBar(jotiOne);
        DrawNowPlayingBar(poppins, poppinsBold, "Hey There Delilah", "Plain White T's", 0.45f, "1:32", "3:52", 0.7f);
        DrawControls(isPlaying);
        DrawNowCard(jotiOne,poppinsBold, poppins, "Hey There Delilah", "Plain White T's");
        DrawSongList(poppins,poppinsBold);
        DrawPlaylistBox(poppinsBold);

        EndTextureMode();

        BeginDrawing();
            ClearBackground(BLACK);
            
            Rectangle sourceRec = { 0.0f, 0.0f, (float)targetTexture.texture.width, 
                                     -(float)targetTexture.texture.height };

            DrawTexturePro(targetTexture.texture, sourceRec, destRec, 
                           (Vector2){0, 0}, 0.0f, WHITE);
        EndDrawing();
    }

    UnloadSongListAssets();
    UnloadTopBarAssets();
    UnloadControlIcons();
    UnloadFont(poppins);
    UnloadFont(jotiOne);
    UnloadRenderTexture(targetTexture);
    CloseWindow();
    return 0;
}