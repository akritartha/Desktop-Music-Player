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
    bool alreadyExists(const std::string &filePath);
    int scanCommonMusicFolders();
    int scanFolders(const std::string &folderPath);
    const std::vector<Song>& allSongs() const;
    const Song* getSongbyId(int id) const;
};