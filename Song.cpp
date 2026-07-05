#include "song.h"
#include <QFileInfo>
Song::Song()
{
    m_id=-1;
    m_duration=0; //qint64 and int doesnt have default values for parameters
}
Song::Song(QString title, QString artist, QString album, QString filePath, qint64 duration){
    m_id=-1; //new object and hasnt been added to database yet, so no id
    // and id same as the default constructor, but the rest of the parameters are set to the given values
    m_title= title;
    m_artist=artist;
    m_album=album;
    m_filePath=filePath;
    m_duration=duration;
}
//getters
QString Song::title() const{
    return m_title;
}
QString Song::album() const{
    return m_album;
}
QString Song::filePath() const{
    return m_filePath;
}
QString Song::artist() const{
    return m_artist;    
}
qint64 Song::duration() const{
    return m_duration;
}
int Song::id() const{
    return m_id;
}
QString Song::durationFormatted() const{
    qint64 totalSeconds= m_duration/1000;
    qint64 minutes= totalSeconds/60;
    qint64 seconds= totalSeconds%60;
    QString secondsStr;
    if (seconds<10){
        secondsStr="0"+QString::number(seconds);
    }
    else{
        secondsStr= QString::number(seconds);
    }
    return QString::number(minutes)+":"+secondsStr;

}
bool Song::isValid() const{
    QFileInfo info(m_filePath);
    bool exists= info.exists();
    return (exists && !m_filePath.isEmpty());
}
//setters
void Song::setTitle(QString title){
    m_title=title;
}
void Song::setArtist(QString artist){
    m_artist=artist;
}
void Song::setAlbum(QString album){
    m_album=album;
}
void Song::setFilePath(QString filePath){
    m_filePath=filePath;
}
void Song::setDuration(qint64 duration){
    m_duration=duration;
}
void Song::setId(int id){
    m_id=id;
}

