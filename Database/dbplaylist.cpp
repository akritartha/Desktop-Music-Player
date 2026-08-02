#include "dbplaylist.h"
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <sstream>
Playlist::Playlist():p_id(count++){}
Playlist::Playlist(std::string name,std::string dbPath):p_name(name),p_dbPath(dbPath),p_id(count++){}
bool Playlist::addSong(const Song &s){

}
bool Playlist::save(){}
bool Playlist::load(){}
bool Playlist::removeSong(int id){}
bool Playlist::alreadyExists(const std::string &filePath){}
const std::vector<Song>& Playlist::allSongs() const{}