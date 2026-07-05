//Declaration of what the class looks like
#pragma once
#include <QString>
class Song{
    private:
        int m_id; //Unique ID from database
        QString m_title;   
        QString m_artist; 
        QString m_album;    
        QString m_filePath;   
        qint64 m_duration;  
    public:
        Song(); //for songs with not all infos given
        Song(QString title, QString artist,QString album,QString filePath, qint64 duration );// for complete songs
        QString title() const; //getter function
        QString album() const;
        QString filePath() const;
        QString artist() const;
        qint64 duration() const;
        QString durationFormatted() const;
        int id() const;
        
        void setTitle(QString title); //setter function
        void setArtist(QString artist);
        void setAlbum(QString album);
        void setFilePath(QString filePath);
        void setDuration(qint64 duration);
        void setId(int id);

        bool isValid() const; //meaning its a constant member function

};