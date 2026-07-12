#include <iostream>
#include "library.h"
#include <fstream>
#include <filesystem>
#include <sstream>
using namespace std;
Library::Library(){}
Library::Library(string dbPath):m_dbPath(dbPath){
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
    return save();
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
    return true;
}
bool Library::removeSong(int id){
    for (int i=0;i<(int)m_songs.size();i++){ //.size() returns size_t type
        if(m_songs[i].id()==id){
            m_songs.erase(m_songs.begin()+i);
            return save();
        }
    }
    return false;
}
const vector<Song>& Library::allSongs() const{
    return m_songs;
}
bool Library::load(){
    if(!filesystem::exists(m_dbPath)){
        return true; //file doesnt exist meaning its empty so empty library
    }
    ifstream file(m_dbPath);
    if(!file.is_open()){
        return false;
    }
    string line;
    while (getline(file,line)){
        stringstream ss(line); //treats a string like a file 
        Song s;
        string id,title,artist,album,path,duration;
        getline(ss,id ,'|');
        try{
            s.setId(stoi(id));
        }catch(...){ //catch any exception (. . .)=ellipsis
            continue; 
            //skip this line if id is not a valid integer
        }
        s.setId(stoi(id));
        getline(ss,title ,'|');
        s.setTitle(title);
        getline(ss,artist ,'|');
        s.setArtist(artist);
        getline(ss,album ,'|');
        s.setAlbum(album);
        getline(ss,path ,'|');
        s.setFilePath(path);
        getline(ss,duration ,'|');
        s.setDuration(stoi(duration));
        m_songs.push_back(s);
    }
    return true;
}


