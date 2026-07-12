#include <iostream>
#include "library.h"
#include <fstream>
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
bool Library::save(){
    ofstream file(m_dbPath); //output file stream for writing
    if(!file.is_open()){
        return false;
    }
    for (const auto& s:m_songs){
        file<<s.id()<<"|"<<s.title()<<"|"
        <<s.artist()<<"|"<<s.album()<<"|"
        <<s.filepath()<<"|"<<s.duration()<<"\n";
    }
}
bool Library::removeSong(int id){
    for (int i=0;i<(int)m_songs.size();i++){ //.size() returns size_t type
        if(m_songs[i].id()==id){
            m_songs.erase(m_songs.begin()+i);
            return true;
        }
    }
    return false;
}

