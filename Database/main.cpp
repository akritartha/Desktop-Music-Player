#include <iostream>
#include "library.h"
#include "song.h"
int main() {

    Library library("library.dat");
    library.scanFolders("/home/kritartha-adhikari/Music");
    library.save();
    Song s("One More Light -Linkin Park.mp3","Linkin Park","One More Light","/home/kritartha-adhikari/Music/One More Light -Linkin Park.mp3");
    std::cout<<library.addSong(s);
    library.save();
}