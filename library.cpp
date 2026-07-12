#include <iostream>
#include "library.h"
using namespace std;
Library::Library(){}
Library::Library(string dbPath):m_dbPath(dbPath){
    m_dbPath="library.txt";
}
bool Library::addSong(const Song& song){
    if(!song.isValid())
        return false;
    for (const auto&s:m_songs){
        //auto lets compiler figure out the type
        //for x in list:
        if (s.filepath()==song.filepath()){
            return false;
        }
    }
    m_songs.push_back(song); //appends song to the end of the vector
    return true;
}

