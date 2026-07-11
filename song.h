#pragma once
#include <string>

class Song{
private:
    std::string m_title;
    std::string m_artist;
    std::string m_album;
    std::string m_filePath;
    int m_id;
    int m_duration;
public:
    Song();
    Song(std::string title,std::string artist, std::string album,
    std::string path,int duration);
    void setTitle(std::string title) ;
    void setArtist(std::string artist) ;
    void setAlbum(std::string album) ;
    void setFilePath(std::string path) ;
    void setDuration(int duration) ;
    void setId(int id);

    std::string title() const;
    std::string artist() const;
    std::string album() const;
    std::string path() const;
    int duration() const;
    int id() const;

    std::string durationFormatted() const;
    bool isValid() const;
};