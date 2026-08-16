#include <iostream>
#include "library.h"
#include <fstream>
#include <filesystem>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <string>
Library::Library() {}
Library::Library(std::string dbPath) : m_dbPath(dbPath)
{
}
bool Library::addSong(const Song &song)
{
    if (!song.isValid() or alreadyExists(song.filepath()))
        return false;
    m_songs.push_back(song); // appends song to the end of the vector
    return save();
}
bool Library::save()
{
    std::ofstream file(m_dbPath); // output file stream for writing
    if (!file.is_open())
    {
        return false;
    }
    for (const auto &s : m_songs)
    {
        file << s.id() << "|" << s.title() << "|"
             << s.artist() << "|" << s.album() << "|"
             << s.filepath() << "|" << s.duration() << "\n";
    }
    return true;
}
bool Library::removeSong(int id)
{
    for (int i = 0; i < static_cast<int>(m_songs.size()); i++)
    { //.size() returns size_t type
        if (m_songs[i].id() == id)
        {
            m_songs.erase(m_songs.begin() + i);
            return save();
        }
    }
    return false;
}
const std::vector<Song> &Library::allSongs() const
{
    return m_songs;
}
bool Library::load()
{ // why load exists is after the program is closed, m_songs becomes empty
    // during each opening of the app, m_songs is reloaded using the load method
    if (!std::filesystem::exists(m_dbPath))
    {
        return true; // file doesnt exist meaning its empty so empty library
    }
    m_songs.clear();
    std::ifstream file(m_dbPath);
    if (!file.is_open())
    {
        return false;
    }
    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line); // treats a string like a file
        Song s;
        std::string id, title, artist, album, path, duration;
        std::getline(ss, id, '|');
        try
        {
            s.setId(std::stoi(id));
        }
        catch (...)
        { // catch any exception (. . .)=ellipsis
            continue;
            // skip this line if id is not a valid integer
        }
        std::getline(ss, title, '|');
        s.setTitle(title);
        std::getline(ss, artist, '|');
        s.setArtist(artist);
        std::getline(ss, album, '|');
        s.setAlbum(album);
        std::getline(ss, path, '|');
        s.setFilePath(path);
        std::getline(ss, duration, '|');
        s.setDuration(std::stoi(duration));
        m_songs.push_back(s);
    }
    return true;
}
bool Library::alreadyExists(const std::string &filePath)
{
    for (const Song &s : m_songs)
    {
        if (s.filepath() == filePath)
        {
            return true;
        }
    }
    return false;
}
int Library::scanFolders(const std::string &folderPath)
{
    namespace f = std::filesystem;
    if (!f::exists(folderPath) || !f::is_directory(folderPath))
    {
        std::cerr << "Invalid folder path: " << folderPath << "\n";
        return 1;
    }
    static const std::vector<std::string> supportedExtensions = {".mp3", ".ogg", ".flac", ".wav"};
    for (const auto &entry : f::directory_iterator(folderPath))
    {
        if (!entry.is_regular_file())
        {
            std::cerr << "Invalid folder path: " << folderPath << "\n";
            return 1;
        }
        std::string ext = entry.path().extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower); // transform(input_start, input_end, destination,operation)
        // transform is within algorithm module
        if (std::find(supportedExtensions.begin(), supportedExtensions.end(), ext) != supportedExtensions.end())
        {
            // ext.end() returns end() if find() finds ext it returns the iterator to the matching element
            std::string filename = entry.path().stem().string();
            Song s(filename, "Unknown Artist", "Unknown Album", entry.path().string());
            addSong(s);
        }
    }
    return 0;
}

const Song *Library::getSongbyId(int id) const
{
    for (const auto &s : m_songs)
    {
        if (s.id() == id)
            return &s;
    }
    return nullptr;
}
