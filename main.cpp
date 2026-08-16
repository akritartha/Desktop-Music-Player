#include "raylib.h"
#include <filesystem>
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
#include <cstdlib>
#include <ctime>
#include "library.h"
#include "song.h"
#include "dbplaylist.h"

SongEntry ToSongEntry(const Song &s)
{
    return SongEntry{s.title(), s.artist(), ""};
}

int FindSongIndexById(const std::vector<Song> &allSongs, int id)
{
    for (int i = 0; i < (int)allSongs.size(); i++)
    {
        if (allSongs[i].id() == id)
            return i;
    }
    return -1;
}

std::vector<int> GetActiveIndices(const std::vector<PlaylistEntry> &playlists, int playlistIdx, int totalSongs)
{
    bool isAllSongs = playlists[playlistIdx].isDefault && playlists[playlistIdx].name == "All Songs";
    if (isAllSongs)
    {
        std::vector<int> all;
        for (int i = 0; i < totalSongs; i++)
            all.push_back(i);
        return all;
    }
    return playlists[playlistIdx].songIndices;
}

enum AdvanceDirection
{
    ADV_NEXT,
    ADV_PREV
};

void AdvanceSong(int &currentSongIndex, const std::vector<PlaylistEntry> &playlists, int currentPlaylistIndex,
                 int totalSongs, bool shuffleOn, AdvanceDirection dir)
{
    std::vector<int> activeIndices = GetActiveIndices(playlists, currentPlaylistIndex, totalSongs);
    if (activeIndices.empty())
        return;

    if (shuffleOn && activeIndices.size() > 1)
    {
        int newIndex;
        do
        {
            newIndex = activeIndices[rand() % activeIndices.size()];
        } while (newIndex == currentSongIndex);
        currentSongIndex = newIndex;
        return;
    }

    auto it = std::find(activeIndices.begin(), activeIndices.end(), currentSongIndex);
    int pos = (it != activeIndices.end()) ? (int)(it - activeIndices.begin()) : 0;
    int size = (int)activeIndices.size();
    pos = (dir == ADV_NEXT) ? (pos + 1) % size : (pos - 1 + size) % size;
    currentSongIndex = activeIndices[pos];
}

int main()
{
    srand((unsigned int)time(nullptr));
    InitWindow(1920, 1080, "Melodex");
    InitAudioDevice();
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowState(FLAG_WINDOW_ALWAYS_RUN);
    SetTargetFPS(60);

    RenderTexture2D targetTexture = LoadRenderTexture(1920, 1080);
    SetTextureFilter(targetTexture.texture, TEXTURE_FILTER_BILINEAR);

    Font jotiOne = LoadFontEx("melodex-gui/assets/Joti_One/JotiOne-Regular.ttf", 64, 0, 0);
    SetTextureFilter(jotiOne.texture, TEXTURE_FILTER_BILINEAR);
    GenTextureMipmaps(&jotiOne.texture);
    Font poppins = LoadFontEx("melodex-gui/assets/Poppins/Poppins-Regular.ttf", 64, 0, 0);
    SetTextureFilter(poppins.texture, TEXTURE_FILTER_BILINEAR);
    GenTextureMipmaps(&poppins.texture);

    Font poppinsBold = LoadFontEx("melodex-gui/assets/Poppins/Poppins-Bold.ttf", 64, 0, 0);
    SetTextureFilter(poppinsBold.texture, TEXTURE_FILTER_BILINEAR);
    GenTextureMipmaps(&poppinsBold.texture);

    LoadControlIcons();
    LoadTopBarAssets();
    LoadSongListAssets();

    bool isPlaying = false;
    bool isRepeatOn = false;
    bool isShuffleOn = false;
    float soundLevel = 0.7f;
    bool isMuted = false;
    float currentSeconds = 0.0f;
    float totalSeconds = 0.0f;
    float songListScroll = 0.0f;
    float lastDraggedPercent = -1.0f;

    Music currentMusic = {0};
    int loadedSongIndex = -1;

    Library library("melodex-gui/musicdb.txt");
    library.load();
    library.scanFolders("melodex-gui/songs");

    std::vector<SongEntry> songs;
    for (const Song &s : library.allSongs())
    {
        songs.push_back(ToSongEntry(s));
    }

    if (songs.empty())
    {
        songs.push_back({"No songs in library", "Add songs to musicdb.txt", ""});
    }

    int currentSongIndex = 0;

    auto LoadSongAudio = [&](int index)
    {
        if (loadedSongIndex != -1 && currentMusic.frameCount > 0)
        {
            StopMusicStream(currentMusic);
            UnloadMusicStream(currentMusic);
        }
        if (index >= 0 && index < (int)library.allSongs().size())
        {
            std::string path = library.allSongs()[index].filepath();
            currentMusic = LoadMusicStream(path.c_str());
            SetMusicVolume(currentMusic, soundLevel);
            loadedSongIndex = index;
            totalSeconds = GetMusicTimeLength(currentMusic);
            currentSeconds = 0.0f;
        }
    };

    LoadSongAudio(currentSongIndex);

    std::string searchText = "";
    bool searchBoxActive = false;

    std::vector<PlaylistEntry> playlists = {
        {"All Songs", "", {}, false, {}, true},
        {"Favorites", "", {}, false, {}, true}};

    std::vector<Playlist> playlistBackends;
    for (const auto &p : playlists)
    {
        std::string filename = "melodex-gui/playlists/" + p.name + ".txt";
        Playlist pl(p.name, filename);
        pl.load();
        playlistBackends.push_back(pl);
    }

    // Convert loaded song IDs back into UI indices
    for (size_t i = 0; i < playlists.size(); i++)
    {
        for (int songId : playlistBackends[i].songIds())
        {
            int idx = FindSongIndexById(library.allSongs(), songId);
            if (idx != -1)
            {
                playlists[i].songIndices.push_back(idx);
            }
        }
    }

    // Load any extra playlists created by the user that aren't in the hardcoded list
    namespace fs = std::filesystem;
    if (fs::exists("melodex-gui/playlists"))
    {
        for (const auto &entry : fs::directory_iterator("melodex-gui/playlists"))
        {
            if (entry.path().extension() == ".txt")
            {
                std::string name = entry.path().stem().string();

                bool alreadyLoaded = false;
                for (const auto &p : playlists)
                {
                    if (p.name == name)
                    {
                        alreadyLoaded = true;
                        break;
                    }
                }

                if (!alreadyLoaded)
                {
                    Playlist pl(name, entry.path().string());
                    pl.load();

                    PlaylistEntry newEntry;
                    newEntry.name = name;
                    newEntry.isDefault = false;
                    for (int songId : pl.songIds())
                    {
                        int idx = FindSongIndexById(library.allSongs(), songId);
                        if (idx != -1)
                            newEntry.songIndices.push_back(idx);
                    }

                    playlists.push_back(newEntry);
                    playlistBackends.push_back(pl);
                }
            }
        }
    }

    float playlistScroll = 0.0f;
    int currentPlaylistIndex = 0; // 0 = "All Songs" by default

    int rightClickedSongIndex = -1;
    Rectangle rightClickedRowRec = {0, 0, 0, 0};
    bool contextMenuOpen = false;
    bool contextMenuShowSubmenu = false;
    int contextMenuSongIndex = -1;
    int rightClickedPlaylistIndex = -1;
    bool deletePlaylistConfirmOpen = false;
    int playlistToDeleteIndex = -1;
    Rectangle contextMenuAnchorRec = {0, 0, 0, 0};
    std::vector<bool> favoriteFlags(songs.size(), false);

    bool showCreatePlaylistPopup = false;
    std::string newPlaylistName = "";
    std::string newPlaylistCoverPath = "";
    bool nameFieldActive = false;
    bool coverFieldActive = false;

    while (!WindowShouldClose())
    {
        float scale = fminf((float)GetScreenWidth() / 1920.0f,
                            (float)GetScreenHeight() / 1080.0f);

        Rectangle destRec = {
            (GetScreenWidth() - (1920.0f * scale)) * 0.5f,
            (GetScreenHeight() - (1080.0f * scale)) * 0.5f,
            1920.0f * scale,
            1080.0f * scale};

        Vector2 mouse = GetMousePosition();
        Vector2 virtualMouse;
        virtualMouse.x = (mouse.x - destRec.x) / scale;
        virtualMouse.y = (mouse.y - destRec.y) / scale;

        if (showCreatePlaylistPopup)
        {
            PopupResult result = GetCreatePlaylistPopupResult(virtualMouse);
            if (result == POPUP_CANCELLED)
            {
                showCreatePlaylistPopup = false;
                newPlaylistName = "";
                newPlaylistCoverPath = "";
            }
            else if (result == POPUP_CREATED)
            {
                if (!newPlaylistName.empty())
                {
                    playlists.push_back({newPlaylistName, newPlaylistCoverPath});
                    std::string filename = "melodex-gui/playlists/" + newPlaylistName + ".txt";
                    Playlist newBackend(newPlaylistName, filename);
                    playlistBackends.push_back(newBackend);
                }
                showCreatePlaylistPopup = false;
                newPlaylistName = "";
                newPlaylistCoverPath = "";
            }
        }

        Rectangle searchBarRec = {82.0f, 148.0f, 569.0f, 60.0f};
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            searchBoxActive = CheckCollisionPointRec(virtualMouse, searchBarRec);
        }

        if (searchBoxActive)
        {
            int key = GetCharPressed();
            while (key > 0)
            {
                if (key >= 32 && key <= 125 && searchText.length() < 60)
                {
                    searchText += (char)key;
                }
                key = GetCharPressed();
            }
            if (IsKeyPressed(KEY_BACKSPACE) && !searchText.empty())
            {
                searchText.pop_back();
            }
        }

        SongEntry currentSong = songs[currentSongIndex];

        soundLevel = UpdateSoundLevel(virtualMouse, soundLevel);
        SetMusicVolume(currentMusic, isMuted ? 0.0f : soundLevel);
        float progressPercent = (totalSeconds > 0) ? (currentSeconds / totalSeconds) : 0.0f;
        float draggedPercent = UpdateSeekPosition(virtualMouse, progressPercent, totalSeconds);

        bool isDragging = IsMouseButtonDown(MOUSE_LEFT_BUTTON) && (draggedPercent != progressPercent);
        if (isDragging)
        {
            progressPercent = draggedPercent;
            lastDraggedPercent = draggedPercent;
        }

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && totalSeconds > 0 && lastDraggedPercent >= 0.0f)
        {
            float newTime = lastDraggedPercent * totalSeconds;
            SeekMusicStream(currentMusic, newTime);
            currentSeconds = newTime;
            lastDraggedPercent = -1.0f;
        }
        if (IsKeyPressed(KEY_UP))
        {
            soundLevel += 0.05f;
            if (soundLevel > 1.0f)
                soundLevel = 1.0f;
            SetMusicVolume(currentMusic, soundLevel);
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            soundLevel -= 0.05f;
            if (soundLevel < 0.0f)
                soundLevel = 0.0f;
            SetMusicVolume(currentMusic, isMuted ? 0.0f : soundLevel);
        }
        if (IsKeyPressed(KEY_M) || (!showCreatePlaylistPopup && IsVolumeButtonClicked(virtualMouse)))
        {
            isMuted = !isMuted;
            if (isMuted)
                SetMusicVolume(currentMusic, 0.0);
            else
                SetMusicVolume(currentMusic, soundLevel);
        }

        if (!showCreatePlaylistPopup)
        {
            if (IsPlayButtonClicked(virtualMouse) || IsKeyPressed(KEY_SPACE))
            {
                isPlaying = !isPlaying;
                if (isPlaying)
                {
                    PlayMusicStream(currentMusic);
                }
                else
                {
                    PauseMusicStream(currentMusic);
                }
            }
        }
        if (!showCreatePlaylistPopup)
        {
            if (IsRepeatButtonClicked(virtualMouse))
            {
                (isShuffleOn) ? (isShuffleOn = !isShuffleOn) : true;
                isRepeatOn = !isRepeatOn;
            }
        }
        if (!showCreatePlaylistPopup)
        {
            if (IsShuffleButtonClicked(virtualMouse))
            {
                (isRepeatOn) ? (isRepeatOn = !isRepeatOn) : true;
                isShuffleOn = !isShuffleOn;
            }
        }
        if (isPlaying)
        {
            UpdateMusicStream(currentMusic);
            currentSeconds = GetMusicTimePlayed(currentMusic);
            if (totalSeconds > 0 && currentSeconds >= totalSeconds - 0.1f)
            {
                if (isRepeatOn)
                {
                    SeekMusicStream(currentMusic, 0.0f);
                    currentSeconds = 0.0f;
                }
                else
                {
                    AdvanceSong(currentSongIndex, playlists, currentPlaylistIndex, (int)songs.size(), isShuffleOn, ADV_NEXT);
                    LoadSongAudio(currentSongIndex);
                    PlayMusicStream(currentMusic);
                }
            }
        }

        if (IsKeyPressed(KEY_RIGHT))
        {
            currentSeconds += 5.0f;
            if (totalSeconds > 0 && currentSeconds >= totalSeconds - 0.1f)
            {
                if (isRepeatOn)
                {
                    currentSeconds = 0.0f;
                    SeekMusicStream(currentMusic, 0.0f);
                }
                else
                {
                    AdvanceSong(currentSongIndex, playlists, currentPlaylistIndex, (int)songs.size(), isShuffleOn, ADV_NEXT);
                    LoadSongAudio(currentSongIndex);
                    if (isPlaying)
                        PlayMusicStream(currentMusic);
                }
            }
            else
            {
                SeekMusicStream(currentMusic, currentSeconds);
            }
        }
        if (IsKeyPressed(KEY_LEFT))
        {
            currentSeconds -= 5.0f;
            if (currentSeconds < 0.0f)
                currentSeconds = 0.0f;
            SeekMusicStream(currentMusic, currentSeconds);
        }

        BeginTextureMode(targetTexture);
        ClearBackground(BLACK);

        DrawRectangleGradientEx(
            (Rectangle){0, 0, 1920, 1080},
            (Color){0, 31, 62, 255},
            (Color){0, 31, 62, 255},
            (Color){0, 115, 230, 255},
            (Color){0, 115, 230, 255});

        DrawTopBar(jotiOne);
        std::string currentTimeStr = FormatTime(currentSeconds);
        std::string totalTimeStr = FormatTime(totalSeconds);
        DrawNowPlayingBar(poppins, poppinsBold, currentSong.title.c_str(), currentSong.artist.c_str(), progressPercent, currentTimeStr.c_str(), totalTimeStr.c_str(), soundLevel);
        DrawControls(isPlaying, isRepeatOn, isShuffleOn, isMuted);
        DrawNowCard(jotiOne, poppinsBold, poppins, currentSong.title.c_str(), currentSong.artist.c_str(),isPlaying);
        bool showAllSongs = playlists[currentPlaylistIndex].isDefault && playlists[currentPlaylistIndex].name == "All Songs";
        rightClickedSongIndex = -1;
        int clickedSong = DrawSongList(poppins, poppinsBold, &songListScroll, virtualMouse, songs, currentSongIndex, searchText, playlists[currentPlaylistIndex].songIndices, showAllSongs, &rightClickedSongIndex, &rightClickedRowRec);

        if (rightClickedSongIndex != -1)
        {
            contextMenuOpen = true;
            contextMenuSongIndex = rightClickedSongIndex;
            contextMenuAnchorRec = rightClickedRowRec;
            contextMenuShowSubmenu = false;
            rightClickedSongIndex = -1;
        }

        if (!showCreatePlaylistPopup)
        {
            if (clickedSong != -1 && clickedSong != currentSongIndex)
            {
                currentSongIndex = clickedSong;
                LoadSongAudio(currentSongIndex);
                if (isPlaying)
                    PlayMusicStream(currentMusic);
            }

            if (IsNextButtonClicked(virtualMouse))
            {
                AdvanceSong(currentSongIndex, playlists, currentPlaylistIndex, (int)songs.size(), isShuffleOn, ADV_NEXT);
                LoadSongAudio(currentSongIndex);
                if (isPlaying)
                    PlayMusicStream(currentMusic);
            }
            if (IsPreviousButtonClicked(virtualMouse))
            {
                AdvanceSong(currentSongIndex, playlists, currentPlaylistIndex, (int)songs.size(), false, ADV_PREV);
                LoadSongAudio(currentSongIndex);
                if (isPlaying)
                    PlayMusicStream(currentMusic);
            }
        }
        rightClickedPlaylistIndex = -1;
        int clickedPlaylist = DrawPlaylistBox(poppinsBold, virtualMouse, &playlistScroll, playlists, &rightClickedPlaylistIndex);

        if (rightClickedPlaylistIndex != -1)
        {
            deletePlaylistConfirmOpen = true;
            playlistToDeleteIndex = rightClickedPlaylistIndex;
            rightClickedPlaylistIndex = -1;
        }

        if (!showCreatePlaylistPopup)
        {
            if (IsAddPlaylistButtonClicked(virtualMouse))
            {
                showCreatePlaylistPopup = true;
            }
            if (clickedPlaylist != -1)
            {
                currentPlaylistIndex = clickedPlaylist;
            }
        }

        if (showCreatePlaylistPopup)
        {
            DrawCreatePlaylistPopup(poppins, poppinsBold, newPlaylistName,
                                    newPlaylistCoverPath, &nameFieldActive,
                                    &coverFieldActive, virtualMouse);
        }
        if (deletePlaylistConfirmOpen && playlistToDeleteIndex != -1)
        {
            Rectangle popupRec = {760.0f, 460.0f, 400.0f, 160.0f};
            DrawRectangleRounded(popupRec, 0.1f, 8, (Color){0, 31, 62, 240});

            std::string msg = "Delete \"" + playlists[playlistToDeleteIndex].name + "\"?";
            DrawTextEx(poppins, msg.c_str(), {popupRec.x + 20.0f, popupRec.y + 20.0f}, 20.0f, 1.0f, WHITE);

            Rectangle yesBtn = {popupRec.x + 40.0f, popupRec.y + 90.0f, 140.0f, 45.0f};
            Rectangle noBtn = {popupRec.x + 220.0f, popupRec.y + 90.0f, 140.0f, 45.0f};

            DrawRectangleRounded(yesBtn, 0.2f, 8, (Color){0, 115, 230, 255});
            DrawTextEx(poppins, "Delete", {yesBtn.x + 35.0f, yesBtn.y + 12.0f}, 18.0f, 1.0f, WHITE);

            DrawRectangleRounded(noBtn, 0.2f, 8, (Color){60, 90, 130, 255});
            DrawTextEx(poppins, "Cancel", {noBtn.x + 35.0f, noBtn.y + 12.0f}, 18.0f, 1.0f, WHITE);

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, yesBtn))
            {
                playlistBackends[playlistToDeleteIndex].deletePlaylistFile();
                playlistBackends.erase(playlistBackends.begin() + playlistToDeleteIndex);
                playlists.erase(playlists.begin() + playlistToDeleteIndex);

                if (currentPlaylistIndex == playlistToDeleteIndex)
                {
                    currentPlaylistIndex = 0;
                }
                else if (currentPlaylistIndex > playlistToDeleteIndex)
                {
                    currentPlaylistIndex--;
                }

                deletePlaylistConfirmOpen = false;
                playlistToDeleteIndex = -1;
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(virtualMouse, noBtn))
            {
                deletePlaylistConfirmOpen = false;
                playlistToDeleteIndex = -1;
            }
        }

        if (contextMenuOpen)
        {
            std::vector<std::string> userPlaylistNames;
            std::vector<int> userPlaylistRealIndices;
            for (size_t p = 0; p < playlists.size(); ++p)
            {
                if (!playlists[p].isDefault)
                {
                    userPlaylistNames.push_back(playlists[p].name);
                    userPlaylistRealIndices.push_back((int)p);
                }
            }

            bool isFav = favoriteFlags[contextMenuSongIndex];
            bool showRemoveOption = !playlists[currentPlaylistIndex].isDefault;
            ContextMenuAction action = DrawSongContextMenu(poppins, contextMenuAnchorRec, isFav, contextMenuShowSubmenu, userPlaylistNames, virtualMouse, showRemoveOption);

            if (action.result == CTX_TOGGLE_FAVORITE)
            {
                favoriteFlags[contextMenuSongIndex] = !favoriteFlags[contextMenuSongIndex];
                auto &favIndices = playlists[1].songIndices;
                int songId = library.allSongs()[contextMenuSongIndex].id();

                if (favoriteFlags[contextMenuSongIndex])
                {
                    favIndices.push_back(contextMenuSongIndex);
                    printf("Adding song ID %d to favorites, save result: %d\n", songId, playlistBackends[1].addSong(songId));
                }
                else
                {
                    favIndices.erase(std::remove(favIndices.begin(), favIndices.end(), contextMenuSongIndex), favIndices.end());
                    playlistBackends[1].removeSong(songId);
                }
                contextMenuOpen = false;
            }
            else if (action.result == CTX_ADD_TO_PLAYLIST_SUBMENU)
            {
                contextMenuShowSubmenu = true;
            }
            else if (action.result == CTX_PLAYLIST_SELECTED)
            {
                int realPlaylistIdx = userPlaylistRealIndices[action.selectedPlaylistIndex];
                playlists[realPlaylistIdx].songIndices.push_back(contextMenuSongIndex);
                int songId = songs.empty() ? -1 : library.allSongs()[contextMenuSongIndex].id();
                if (songId != -1)
                {
                    playlistBackends[realPlaylistIdx].addSong(songId);
                }
                contextMenuOpen = false;
            }
            else if (action.result == CTX_REMOVE_FROM_PLAYLIST)
            {
                int songId = library.allSongs()[contextMenuSongIndex].id();
                playlistBackends[currentPlaylistIndex].removeSong(songId);

                auto &indices = playlists[currentPlaylistIndex].songIndices;
                indices.erase(std::remove(indices.begin(), indices.end(), contextMenuSongIndex), indices.end());

                contextMenuOpen = false;
            }
            else if (action.result == CTX_CLOSED)
            {
                contextMenuOpen = false;
            }
        }

        EndTextureMode();

        BeginDrawing();
        ClearBackground(BLACK);

        Rectangle sourceRec = {0.0f, 0.0f, (float)targetTexture.texture.width,
                               -(float)targetTexture.texture.height};

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
    CloseAudioDevice();
    return 0;
}