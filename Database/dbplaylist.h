#pragma once
#include <string>
#include <vector>
#include "song.h"

class Playlist{
    private:
        static int count;
        int p_id;
        std::string p_name;
        std::vector<int> p_songId;
        std::string p_dbPath;
    public:
        Playlist();
        Playlist(std::string name ,std::string dbPath);
        bool addSong(int songId);
        bool save();
        bool load();
        bool removeSong(int songId);
        bool deletePlaylistFile();
        bool alreadyExists(int songId);
        const std::vector<int>& songIds() const;
};