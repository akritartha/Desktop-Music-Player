#include "song.h"
using namespace std;

Song::Song(){}

Song::Song(std::string title, std::string artist, std::string album, std::string path, int duration)
    : m_title(title), m_artist(artist), m_album(album), m_filePath(path), m_duration(duration) {
    static int nextId = 1;
    m_id = nextId++;
}

void Song::setTitle(std::string title) const {
    m_title = title;
}

void Song::setArtist(std::string artist) const {
    m_artist = artist;
}

void Song::setAlbum(std::string album) const {
    m_album = album;
}

void Song::setPath(std::string path) const {
    m_filePath = path;
}

void Song::setDuration(int duration) const {
    m_duration = duration;
}

void Song::setId(int id) const {
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

std::string Song::path() const {
    return m_filePath;
}

int Song::duration() const {
    return m_duration;
}

int Song::id() const {
    return m_id;
}

std::string Song::durationFormatted() const {
    int minutes = duration() / 60;
    int seconds = duration() % 60;
    return std::to_string(minutes) + ":" + (seconds < 10 ? "0" : "") + std::to_string(seconds);
}

bool Song::isValid() const {
    return !title().empty() && !artist().empty() && !album().empty() && !path().empty();
}