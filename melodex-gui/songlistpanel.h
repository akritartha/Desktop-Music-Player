#ifndef SONGLISTPANEL_H
#define SONGLISTPANEL_H

#include "scrollablepanel.h"
#include "songlist.h"
#include <vector>
#include <string>

class SongListPanel : public ScrollablePanel {
private:
    Font poppinsFont;
    Font poppinsFontBold;
    Vector2 virtualMouse;
    const std::vector<SongEntry>& songs;
    int currentSongIndex;
    std::string searchText;
    const std::vector<int>& activePlaylistSongIndices;
    bool showAllSongs;
    int* rightClickedSongIndex;
    Rectangle* rightClickedRowRec;
    int clickedIndex;

public:
    SongListPanel(Rectangle b, Font poppins, Font poppinsBold, Vector2 mouse, 
                  const std::vector<SongEntry>& s, int curIdx, const std::string& search, 
                  const std::vector<int>& activeInd, bool showAll, 
                  int* rClickIdx, Rectangle* rClickRec);
    
    void Draw() override;
    int GetClickedIndex() const;
};

#endif // SONGLISTPANEL_H
