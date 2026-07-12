#pragma once
#include <string>
#include <vector>
#include "song.h"
class Library{
private:
    std::vector<Song> m_songs;
    std::string m_dbPath;
public:
    Library();
    Library(std::string dbPath);
    bool addSong(const Song& s);
    bool save();
    bool removeSong(int id); // id is primary key
    bool load();
};