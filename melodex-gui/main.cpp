#include "raylib.h"
#include <cstdio>
#include <math.h>
#include "topbar.h"
#include "nowplaying.h"
#include "controls.h"
#include "nowcard.h"
#include "songlist.h"
#include "playlistbox.h"
#include "createplaylistpopup.h"
#include "songcontextmenu.h"
#include "textutils.h"
#include <vector>
#include <string>
#include <algorithm>


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
    float currentSeconds = 92.0f;   // 1:32 starting point, placeholder
    float totalSeconds = 232.0f;    // 3:52 total, placeholder (will come from DB later)
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

    std::vector<PlaylistEntry> playlists = {
        { "All Songs", "", {}, false, {}, true },
        { "Favorites", "", {}, false, {}, true },
        { "English", "", {}, false, {}, false },
        { "Hindi", "", {}, false, {}, false },
        { "Nepali", "", {}, false, {}, false },
        { "Spanish", "", {}, false, {}, false }
    };
    float playlistScroll = 0.0f;
    int currentPlaylistIndex = 0;  // 0 = "All Songs" by default

    int rightClickedSongIndex = -1;
    Rectangle rightClickedRowRec = { 0, 0, 0, 0 };
    bool contextMenuOpen = false;
    bool contextMenuShowSubmenu = false;
    int contextMenuSongIndex = -1;
    Rectangle contextMenuAnchorRec = { 0, 0, 0, 0 };
    std::vector<bool> favoriteFlags(songs.size(), false);

    bool showCreatePlaylistPopup = false;
    std::string newPlaylistName = "";
    std::string newPlaylistCoverPath = "";
    bool nameFieldActive = false;
    bool coverFieldActive = false;

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

        if (showCreatePlaylistPopup) {
            PopupResult result = GetCreatePlaylistPopupResult(virtualMouse);
            if (result == POPUP_CANCELLED) {
                showCreatePlaylistPopup = false;
                newPlaylistName = "";
                newPlaylistCoverPath = "";
            } else if (result == POPUP_CREATED) {
                if (!newPlaylistName.empty()) {
                    playlists.push_back({ newPlaylistName, newPlaylistCoverPath });
                }
                showCreatePlaylistPopup = false;
                newPlaylistName = "";
                newPlaylistCoverPath = "";
            }
        }

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

        float progressPercent = currentSeconds / totalSeconds;
        progressPercent = UpdateSeekPosition(virtualMouse, progressPercent, totalSeconds);
        currentSeconds = progressPercent * totalSeconds;

        if (IsKeyPressed(KEY_UP)) {
            soundLevel += 0.05f;
            if (soundLevel > 1.0f) soundLevel = 1.0f;
        }
        if (IsKeyPressed(KEY_DOWN)) {
            soundLevel -= 0.05f;
            if (soundLevel < 0.0f) soundLevel = 0.0f;
        }

        if (!showCreatePlaylistPopup) {
            if (IsPlayButtonClicked(virtualMouse)) {
                isPlaying = !isPlaying;
                printf("Play button clicked, isPlaying = %s\n", isPlaying ? "true" : "false");
            }
        }

        if (isPlaying) {
            currentSeconds += GetFrameTime();
            if (currentSeconds >= totalSeconds) {
                currentSeconds = 0.0f;
                currentSongIndex++;
                if (currentSongIndex >= (int)songs.size()) currentSongIndex = 0;
            }
        }

        if (IsKeyPressed(KEY_RIGHT)) {
            currentSeconds += 5.0f;
            if (currentSeconds > totalSeconds) currentSeconds = totalSeconds;
        }
        if (IsKeyPressed(KEY_LEFT)) {
            currentSeconds -= 5.0f;
            if (currentSeconds < 0.0f) currentSeconds = 0.0f;
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
        std::string currentTimeStr = FormatTime(currentSeconds);
        std::string totalTimeStr = FormatTime(totalSeconds);
        DrawNowPlayingBar(poppins, poppinsBold, currentSong.title.c_str(), currentSong.artist.c_str(), progressPercent, currentTimeStr.c_str(), totalTimeStr.c_str(), soundLevel);
        DrawControls(isPlaying);
        DrawNowCard(jotiOne, poppinsBold, poppins, currentSong.title.c_str(), currentSong.artist.c_str());
        bool showAllSongs = playlists[currentPlaylistIndex].isDefault && playlists[currentPlaylistIndex].name == "All Songs";
        rightClickedSongIndex = -1;
        int clickedSong = DrawSongList(poppins, poppinsBold, &songListScroll, virtualMouse, songs, currentSongIndex, searchText, playlists[currentPlaylistIndex].songIndices, showAllSongs, &rightClickedSongIndex, &rightClickedRowRec);

        if (rightClickedSongIndex != -1) {
            contextMenuOpen = true;
            contextMenuSongIndex = rightClickedSongIndex;
            contextMenuAnchorRec = rightClickedRowRec;
            contextMenuShowSubmenu = false;
            rightClickedSongIndex = -1;
        }
        
        if (!showCreatePlaylistPopup) {
            if (clickedSong != -1 && clickedSong != currentSongIndex) {
                currentSongIndex = clickedSong;
                currentSeconds = 0.0f;
            }
            
            if (IsNextButtonClicked(virtualMouse)) {
                currentSongIndex++;
                if (currentSongIndex >= (int)songs.size()) currentSongIndex = 0;
                currentSeconds = 0.0f;
            }
            if (IsPreviousButtonClicked(virtualMouse)) {
                currentSongIndex--;
                if (currentSongIndex < 0) currentSongIndex = (int)songs.size() - 1;
                currentSeconds = 0.0f;
            }
        }
        int clickedPlaylist = DrawPlaylistBox(poppinsBold, virtualMouse, &playlistScroll, playlists);
        
        if (!showCreatePlaylistPopup) {
            if (IsAddPlaylistButtonClicked(virtualMouse)) {
                showCreatePlaylistPopup = true;
            }
            if (clickedPlaylist != -1) {
                currentPlaylistIndex = clickedPlaylist;
            }
        }

        if (showCreatePlaylistPopup) {
            DrawCreatePlaylistPopup(poppins, poppinsBold, newPlaylistName, 
                                     newPlaylistCoverPath, &nameFieldActive, 
                                     &coverFieldActive, virtualMouse);
        }

        if (contextMenuOpen) {
            std::vector<std::string> userPlaylistNames;
            std::vector<int> userPlaylistRealIndices;
            for (size_t p = 0; p < playlists.size(); ++p) {
                if (!playlists[p].isDefault) {
                    userPlaylistNames.push_back(playlists[p].name);
                    userPlaylistRealIndices.push_back((int)p);
                }
            }

            bool isFav = favoriteFlags[contextMenuSongIndex];
            ContextMenuAction action = DrawSongContextMenu(poppins, contextMenuAnchorRec, isFav, contextMenuShowSubmenu, userPlaylistNames, virtualMouse);

            if (action.result == CTX_TOGGLE_FAVORITE) {
                favoriteFlags[contextMenuSongIndex] = !favoriteFlags[contextMenuSongIndex];
                auto& favIndices = playlists[1].songIndices;
                if (favoriteFlags[contextMenuSongIndex]) {
                    favIndices.push_back(contextMenuSongIndex);
                } else {
                    favIndices.erase(std::remove(favIndices.begin(), favIndices.end(), contextMenuSongIndex), favIndices.end());
                }
                contextMenuOpen = false;
            } else if (action.result == CTX_ADD_TO_PLAYLIST_SUBMENU) {
                contextMenuShowSubmenu = true;
            } else if (action.result == CTX_PLAYLIST_SELECTED) {
                int realPlaylistIdx = userPlaylistRealIndices[action.selectedPlaylistIndex];
                playlists[realPlaylistIdx].songIndices.push_back(contextMenuSongIndex);
                contextMenuOpen = false;
            } else if (action.result == CTX_CLOSED) {
                contextMenuOpen = false;
            }
        }
 
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