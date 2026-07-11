#include "song.h"
#include <filesystem>
#include <iostream>

using namespace std;

Song::Song(){}

Song::Song(std::string title, std::string artist, std::string album, std::string path, int duration)
    : m_title(title), m_artist(artist), m_album(album), m_filePath(path), m_duration(duration) {
    static int nextId = 1;
    m_id = nextId++;
}

void Song::setTitle(std::string title)  {
    m_title = title;
}

void Song::setArtist(std::string artist)  {
    m_artist = artist;
}

void Song::setAlbum(std::string album)  {
    m_album = album;
}

void Song::setFilePath(std::string path)  {
    m_filePath = path;
}

void Song::setDuration(int duration)  {
    m_duration = duration;
}

void Song::setId(int id)  {
    m_id = id;
}

std::string Song::title() const {
    return m_title;
}

std::string Song::artist() const {
    return m_artist;
}

std::string Song::album() const {
    return m_album;
}

std::string Song::filepath() const {
    return m_filePath;
}

int Song::duration() const {
    return m_duration;
}

int Song::id() const {
    return m_id;
}

std::string Song::durationFormatted() const {
    int totalSeconds= duration()/1000;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;
    return std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
}

bool Song::isValid() const {
    return !filepath().empty() && std::filesystem::exists(m_filePath);
}
