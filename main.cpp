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
#include "addfolderpopup.h"
#include "confirmdialog.h"
#include "songcontextmenu.h"
#include "textutils.h"
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "library.h"
#include "song.h"
#include "dbplaylist.h"

SongEntry ToSongEntry(const Song &s)
{
    return SongEntry(s.title(), s.artist(), "");
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
    bool isAllSongs = playlists[playlistIdx].isDefault() && playlists[playlistIdx].name() == "All Songs";
    if (isAllSongs)
    {
        std::vector<int> all;
        for (int i = 0; i < totalSongs; i++)
            all.push_back(i);
        return all;
    }
    return playlists[playlistIdx].songIndices();
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

class MusicPlayerApp
{
public:
    Library library;
    std::vector<SongEntry> songs;
    std::vector<PlaylistEntry> playlists;
    std::vector<Playlist> playlistBackends;
    std::vector<std::string> extraScannedFolders;
    std::vector<bool> favoriteFlags;

    Music currentMusic = {0};
    int loadedSongIndex = -1;
    int currentSongIndex = 0;
    int currentPlaylistIndex = 0;

    bool isPlaying = false;
    bool isRepeatOn = false;
    bool isShuffleOn = false;
    bool isMuted = false;
    float soundLevel = 0.7f;
    float currentSeconds = 0.0f;
    float totalSeconds = 0.0f;
    float lastDraggedPercent = -1.0f;

    std::string searchText = "";
    bool searchBoxActive = false;
    float songListScroll = 0.0f;
    float playlistScroll = 0.0f;

    int rightClickedSongIndex = -1;
    Rectangle rightClickedRowRec = {0, 0, 0, 0};
    bool contextMenuOpen = false;
    bool contextMenuShowSubmenu = false;
    int contextMenuSongIndex = -1;
    Rectangle contextMenuAnchorRec = {0, 0, 0, 0};

    int rightClickedPlaylistIndex = -1;
    bool deletePlaylistConfirmOpen = false;
    int playlistToDeleteIndex = -1;

    bool showCreatePlaylistPopup = false;
    std::string newPlaylistName = "";
    std::string newPlaylistCoverPath = "";
    bool nameFieldActive = false;
    bool coverFieldActive = false;

    bool showAddFolderPopup = false;
    std::string newFolderPath = "";
    bool folderFieldActive = false;

    std::string extraFoldersConfigPath = "melodex-gui/scanned_folders.txt";

    MusicPlayerApp(const std::string& dbPath) : library(dbPath) {}
    

    void Init()
    {
        library.load();
        library.scanFolders("melodex-gui/songs");

        {
            std::ifstream cfgIn(extraFoldersConfigPath);
            if (cfgIn.is_open())
            {
                std::string line;
                while (std::getline(cfgIn, line))
                {
                    if (line.empty())
                        continue;
                    extraScannedFolders.push_back(line);
                    library.scanFolders(line);
                }
            }
        }

        for (const Song &s : library.allSongs())
        {
            songs.push_back(ToSongEntry(s));
        }
        if (songs.empty())
        {
            songs.push_back({"No songs in library", "Add songs to musicdb.txt", ""});
        }

        playlists.push_back(PlaylistEntry("All Songs", "", true));
        playlists.push_back(PlaylistEntry("Favorites", "", true));

        for (const auto &p : playlists)
        {
            std::string filename = "melodex-gui/playlists/" + p.name() + ".txt";
            Playlist pl(p.name(), filename);
            pl.load();
            playlistBackends.push_back(pl);
        }

        for (size_t i = 0; i < playlists.size(); i++)
        {
            for (int songId : playlistBackends[i].songIds())
            {
                int idx = FindSongIndexById(library.allSongs(), songId);
                if (idx != -1)
                {
                    playlists[i].addSongIndex(idx);
                }
            }
        }

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
                        if (p.name() == name)
                        {
                            alreadyLoaded = true;
                            break;
                        }
                    }

                    if (!alreadyLoaded)
                    {
                        Playlist pl(name, entry.path().string());
                        pl.load();

                        PlaylistEntry newEntry(name, "", false);
                        for (int songId : pl.songIds())
                        {
                            int idx = FindSongIndexById(library.allSongs(), songId);
                            if (idx != -1)
                                newEntry.addSongIndex(idx);
                        }

                        playlists.push_back(newEntry);
                        playlistBackends.push_back(pl);
                    }
                }
            }
        }

        favoriteFlags.resize(songs.size(), false);
    }


    // Loads audio for the song at the given index, replacing whatever is currently loaded
    void LoadSongAudio(int index)
    {
        // stop and free the previous song's audio, if any
        if (loadedSongIndex != -1 && currentMusic.frameCount > 0)
        {
            StopMusicStream(currentMusic);
            UnloadMusicStream(currentMusic);
        }

        // load new song and reset playback position
        if (index >= 0 && index < (int)library.allSongs().size())
        {
            std::string path = library.allSongs()[index].filepath();
            currentMusic = LoadMusicStream(path.c_str());
            SetMusicVolume(currentMusic, soundLevel);
            loadedSongIndex = index;
            totalSeconds = GetMusicTimeLength(currentMusic);
            currentSeconds = 0.0f;
        }
    }
    // Handles play/pause, shuffle, repeat, mute toggles, and volume/seek input.
// Assumes virtualMouse and popup-blocking flags are passed in each frame.
    void HandlePlaybackInput(Vector2 virtualMouse, bool popupsOpen,
                          IconButton* playBtn, IconButton* repeatBtn, IconButton* shuffleBtn, IconButton* volumeBtn)
   {
     // volume slider + drag-based seek stay here since they don't depend on popups
    soundLevel = UpdateSoundLevel(virtualMouse, soundLevel);
    SetMusicVolume(currentMusic, isMuted ? 0.0f : soundLevel);

    float progressPercent = (totalSeconds > 0) ? (currentSeconds / totalSeconds) : 0.0f;
    float draggedPercent = UpdateSeekPosition(virtualMouse, progressPercent, totalSeconds);

    bool isDragging = IsMouseButtonDown(MOUSE_LEFT_BUTTON) && (draggedPercent != progressPercent);
    if (isDragging)
    {
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
        if (soundLevel > 1.0f) soundLevel = 1.0f;
        SetMusicVolume(currentMusic, isMuted ? 0.0f : soundLevel);
    }
    if (IsKeyPressed(KEY_DOWN))
    {
        soundLevel -= 0.05f;
        if (soundLevel < 0.0f) soundLevel = 0.0f;
        SetMusicVolume(currentMusic, isMuted ? 0.0f : soundLevel);
    }

    if (IsKeyPressed(KEY_M) || (!popupsOpen && volumeBtn->IsClicked(virtualMouse)))
    {
        isMuted = !isMuted;
        SetMusicVolume(currentMusic, isMuted ? 0.0f : soundLevel);
    }

    if (!popupsOpen)
    {
        if (playBtn->IsClicked(virtualMouse) || IsKeyPressed(KEY_SPACE))
        {
            isPlaying = !isPlaying;
            if (isPlaying) PlayMusicStream(currentMusic);
            else PauseMusicStream(currentMusic);
        }
        if (repeatBtn->IsClicked(virtualMouse))
        {
            if (isShuffleOn) isShuffleOn = false;
            isRepeatOn = !isRepeatOn;
        }
        if (shuffleBtn->IsClicked(virtualMouse))
        {
            if (isRepeatOn) isRepeatOn = false;
            isShuffleOn = !isShuffleOn;
        }
    }
}

};

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

// Create and initialize the app 
    MusicPlayerApp app("melodex-gui/musicdb.txt");
    app.Init();
    app.LoadSongAudio(app.currentSongIndex);

    Library& library = app.library;
    std::vector<SongEntry>& songs = app.songs;
    std::vector<PlaylistEntry>& playlists = app.playlists;
    std::vector<Playlist>& playlistBackends = app.playlistBackends;
    std::vector<bool>& favoriteFlags = app.favoriteFlags;
    Music& currentMusic = app.currentMusic;
    int& loadedSongIndex = app.loadedSongIndex;
    int& currentSongIndex = app.currentSongIndex;
    bool& isPlaying = app.isPlaying;
    bool& isRepeatOn = app.isRepeatOn;
    bool& isShuffleOn = app.isShuffleOn;
    bool& isMuted = app.isMuted;
    float& soundLevel = app.soundLevel;
    float& currentSeconds = app.currentSeconds;
    float& totalSeconds = app.totalSeconds;
    float& lastDraggedPercent = app.lastDraggedPercent;
    std::string& searchText = app.searchText;
    bool& searchBoxActive = app.searchBoxActive;
    float& songListScroll = app.songListScroll;
    float& playlistScroll = app.playlistScroll;
    std::vector<std::string>& extraScannedFolders = app.extraScannedFolders;
    std::string& extraFoldersConfigPath = app.extraFoldersConfigPath;

    auto LoadSongAudio = [&](int index) { app.LoadSongAudio(index); };
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

    bool showCreatePlaylistPopup = false;
    std::string newPlaylistName = "";
    std::string newPlaylistCoverPath = "";
    bool nameFieldActive = false;
    bool coverFieldActive = false;

    bool showAddFolderPopup = false;
    std::string newFolderPath = "";
    bool folderFieldActive = false;

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
                    playlists.push_back(PlaylistEntry(newPlaylistName, newPlaylistCoverPath));
                    std::string filename = "melodex-gui/playlists/" + newPlaylistName + ".txt";
                    Playlist newBackend(newPlaylistName, filename);
                    playlistBackends.push_back(newBackend);
                }
                showCreatePlaylistPopup = false;
                newPlaylistName = "";
                newPlaylistCoverPath = "";
            }
        }

        if (showAddFolderPopup)
        {
            AddFolderResult folderResult = GetAddFolderPopupResult(virtualMouse);
            if (folderResult == ADDFOLDER_CANCELLED)
            {
                showAddFolderPopup = false;
                newFolderPath = "";
            }
            else if (folderResult == ADDFOLDER_ADDED)
            {
                namespace fs = std::filesystem;
                if (!newFolderPath.empty() && fs::exists(newFolderPath) && fs::is_directory(newFolderPath))
                {
                    library.scanFolders(newFolderPath);

                    // Rebuild the UI-facing songs list from the library
                    songs.clear();
                    for (const Song &s : library.allSongs())
                    {
                        songs.push_back(ToSongEntry(s));
                    }
                    favoriteFlags.resize(songs.size(), false);

                    // Remember this folder for next launch, avoiding duplicates
                    bool alreadySaved = false;
                    for (const auto &f : extraScannedFolders)
                    {
                        if (f == newFolderPath)
                        {
                            alreadySaved = true;
                            break;
                        }
                    }
                    if (!alreadySaved)
                    {
                        extraScannedFolders.push_back(newFolderPath);
                        std::ofstream cfgOut(extraFoldersConfigPath, std::ios::app);
                        if (cfgOut.is_open())
                        {
                            cfgOut << newFolderPath << "\n";
                        }
                    }
                }
                showAddFolderPopup = false;
                newFolderPath = "";
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
        if (IsKeyPressed(KEY_M) || (!showCreatePlaylistPopup && !showAddFolderPopup && volumeBtn->IsClicked(virtualMouse)))
        {
            isMuted = !isMuted;
            if (isMuted)
                SetMusicVolume(currentMusic, 0.0);
            else
                SetMusicVolume(currentMusic, soundLevel);
        }

        if (!showCreatePlaylistPopup && !showAddFolderPopup)
        {
            if (playBtn->IsClicked(virtualMouse) || IsKeyPressed(KEY_SPACE))
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
        if (!showCreatePlaylistPopup && !showAddFolderPopup)
        {
            if (repeatBtn->IsClicked(virtualMouse))
            {
                (isShuffleOn) ? (isShuffleOn = !isShuffleOn) : true;
                isRepeatOn = !isRepeatOn;
            }
        }
        if (!showCreatePlaylistPopup && !showAddFolderPopup)
        {
            if (shuffleBtn->IsClicked(virtualMouse))
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
        DrawNowPlayingBar(poppins, poppinsBold, currentSong.title().c_str(), currentSong.artist().c_str(), progressPercent, currentTimeStr.c_str(), totalTimeStr.c_str(), soundLevel);
        DrawControls(isPlaying, isRepeatOn, isShuffleOn, isMuted);
        DrawNowCard(jotiOne, poppinsBold, poppins, currentSong.title().c_str(), currentSong.artist().c_str(),isPlaying);
        bool showAllSongs = playlists[currentPlaylistIndex].isDefault() && playlists[currentPlaylistIndex].name() == "All Songs";
        rightClickedSongIndex = -1;
        int clickedSong = DrawSongList(poppins, poppinsBold, &songListScroll, virtualMouse, songs, currentSongIndex, searchText, playlists[currentPlaylistIndex].songIndices(), showAllSongs, &rightClickedSongIndex, &rightClickedRowRec);

        if (rightClickedSongIndex != -1)
        {
            contextMenuOpen = true;
            contextMenuSongIndex = rightClickedSongIndex;
            contextMenuAnchorRec = rightClickedRowRec;
            contextMenuShowSubmenu = false;
            rightClickedSongIndex = -1;
        }

        if (!showCreatePlaylistPopup && !showAddFolderPopup)
        {
            if (clickedSong != -1 && clickedSong != currentSongIndex)
            {
                currentSongIndex = clickedSong;
                LoadSongAudio(currentSongIndex);
                if (isPlaying)
                    PlayMusicStream(currentMusic);
            }

            if (nextBtn->IsClicked(virtualMouse))
            {
                AdvanceSong(currentSongIndex, playlists, currentPlaylistIndex, (int)songs.size(), isShuffleOn, ADV_NEXT);
                LoadSongAudio(currentSongIndex);
                if (isPlaying)
                    PlayMusicStream(currentMusic);
            }
            if (prevBtn->IsClicked(virtualMouse))
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

        if (!showCreatePlaylistPopup && !showAddFolderPopup)
        {
            if (IsAddPlaylistButtonClicked(virtualMouse))
            {
                showCreatePlaylistPopup = true;
            }
            if (IsAddFolderButtonClicked(virtualMouse))
            {
                showAddFolderPopup = true;
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
        if (showAddFolderPopup)
        {
            DrawAddFolderPopup(poppins, poppinsBold, newFolderPath,
                                &folderFieldActive, virtualMouse);
        }
        if (deletePlaylistConfirmOpen && playlistToDeleteIndex != -1)
        {
            Rectangle popupRec = {760.0f, 460.0f, 400.0f, 160.0f};
            std::string msg = "Delete \"" + playlists[playlistToDeleteIndex].name() + "\"?";
            
            ConfirmDialog dialog(popupRec, poppins, poppinsBold, msg, virtualMouse);
            dialog.Draw();

            ConfirmResult res = dialog.GetResult();
            if (res == CONFIRM_YES)
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
            else if (res == CONFIRM_NO)
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
                if (!playlists[p].isDefault())
                {
                    userPlaylistNames.push_back(playlists[p].name());
                    userPlaylistRealIndices.push_back((int)p);
                }
            }

            bool isFav = favoriteFlags[contextMenuSongIndex];
            bool showRemoveOption = !playlists[currentPlaylistIndex].isDefault();
            ContextMenuAction action = DrawSongContextMenu(poppins, contextMenuAnchorRec, isFav, contextMenuShowSubmenu, userPlaylistNames, virtualMouse, showRemoveOption);

            if (action.result == CTX_TOGGLE_FAVORITE)
            {
                favoriteFlags[contextMenuSongIndex] = !favoriteFlags[contextMenuSongIndex];
                int songId = library.allSongs()[contextMenuSongIndex].id();

                if (favoriteFlags[contextMenuSongIndex])
                {
                    playlists[1].addSongIndex(contextMenuSongIndex);
                    printf("Adding song ID %d to favorites, save result: %d\n", songId, playlistBackends[1].addSong(songId));
                }
                else
                {
                    playlists[1].removeSongIndex(contextMenuSongIndex);
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
                playlists[realPlaylistIdx].addSongIndex(contextMenuSongIndex);
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

                playlists[currentPlaylistIndex].removeSongIndex(contextMenuSongIndex);

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