#pragma once
#include <string>
#include <vector>
#include "song.h"

class Playlist{
    private:
        static int count;
        int p_id;
        std::string p_name;
        std::vector<Song> p_songs;
        std::string p_dbPath;
    public:
        Playlist();
        Playlist(std::string name ,std::string dbPath);
        bool addSong(const Song& s);
        bool save();
        bool load();
        bool removeSong(int id);
        bool alreadyExists(const std::string &filePath);
        const std::vector<Song>& allSongs() const;
};