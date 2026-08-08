#include "dbplaylist.h"
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <sstream>
int Playlist::count =1;
Playlist::Playlist():p_id(count++){}
Playlist::Playlist(std::string name,std::string dbPath):p_name(name),p_dbPath(dbPath),p_id(count++){}
bool Playlist::addSong(int songId){
    if(alreadyExists(songId))
        return false;
    p_songId.push_back(songId);
    return save();
}
bool Playlist::save(){
    std::ofstream file(p_dbPath);
    if(!file.is_open())
        return false;
    for (const auto& s:p_songId){
        file<<s<<"\n";
    }
    return true;
    
}
bool Playlist::load(){
    if(!std::filesystem::exists(p_dbPath))
        return true;
    p_songId.clear();
    std::ifstream file(p_dbPath);
    if(!file.is_open())
        return false;
    std::string line;
    while(std::getline(file,line)){
        try{
            p_songId.push_back(std::stoi(line));
        }
        catch(...){
            continue;
        }
    }
    return true;
}
bool Playlist::removeSong(int id){
    for(int i=0;i< static_cast<int>(p_songId.size());i++){
        if(p_songId[i]==id){
            p_songId.erase(p_songId.begin()+i);
            return save();
        }
    }
    return false;
}
bool Playlist::alreadyExists(int songId){
    for(int i=0;i< static_cast<int>(p_songId.size());i++){
        if(p_songId[i]==songId){
            return true;
        }
    }
    return false;
}
const std::vector<int>& Playlist::songIds() const{}