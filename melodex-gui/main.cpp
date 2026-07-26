#include "raylib.h"
#include <cstdio>
#include <math.h>
#include "topbar.h"
#include "nowplaying.h"
#include "controls.h"
#include "nowcard.h"
#include "songlist.h"
#include "playlistbox.h"
#include <vector>
#include <string>

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
    float soundLevel = 0.7f;
    float songListScroll = 0.0f;

    std::vector<SongEntry> songs = {
        { "Hey There Delilah", "Plain White T's", "" },
        { "Blue", "Yung Kai", "" },
        { "Test Song 3", "Artist 3", "" },
        { "Test Song 4", "Artist 4", "" },
        { "Test Song 5", "Artist 5", "" },
        { "Test Song 6", "Artist 6", "" },
        { "Test Song 7", "Artist 7", "" }
    };
    int currentSongIndex = 0;
    std::string searchText = "";
    bool searchBoxActive = false;

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

        Rectangle searchBarRec = { 82.0f, 148.0f, 569.0f, 60.0f };
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            searchBoxActive = CheckCollisionPointRec(virtualMouse, searchBarRec);
        }

        if (searchBoxActive) {
            int key = GetCharPressed();
            while (key > 0) {
                if (key >= 32 && key <= 125 && searchText.length() < 60) {
                    searchText += (char)key;
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && !searchText.empty()) {
                searchText.pop_back();
            }
        }

        

        SongEntry currentSong = songs[currentSongIndex];

        soundLevel = UpdateSoundLevel(virtualMouse, soundLevel);

        if (IsKeyPressed(KEY_UP)) {
            soundLevel += 0.05f;
            if (soundLevel > 1.0f) soundLevel = 1.0f;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            soundLevel -= 0.05f;
            if (soundLevel < 0.0f) soundLevel = 0.0f;
        }

        if (IsPlayButtonClicked(virtualMouse)) {
            isPlaying = !isPlaying;
            printf("Play button clicked, isPlaying = %s\n", isPlaying ? "true" : "false");
        }

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
        DrawNowPlayingBar(poppins, poppinsBold, currentSong.title.c_str(), currentSong.artist.c_str(), 0.45f, "1:32", "3:52", soundLevel);
        DrawControls(isPlaying);
        DrawNowCard(jotiOne, poppinsBold, poppins, currentSong.title.c_str(), currentSong.artist.c_str());
        int clickedSong = DrawSongList(poppins, poppinsBold, &songListScroll, virtualMouse, songs, currentSongIndex, searchText);
        
        if (clickedSong != -1) {
            currentSongIndex = clickedSong;
        }
        
        if (IsNextButtonClicked(virtualMouse)) {
            currentSongIndex++;
            if (currentSongIndex >= (int)songs.size()) currentSongIndex = 0;
        }
        if (IsPreviousButtonClicked(virtualMouse)) {
            currentSongIndex--;
            if (currentSongIndex < 0) currentSongIndex = (int)songs.size() - 1;
        }
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