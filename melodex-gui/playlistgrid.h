#ifndef PLAYLISTGRID_H
#define PLAYLISTGRID_H

#include "scrollablepanel.h"
#include "playlistbox.h"
#include <vector>

class PlaylistGrid : public ScrollablePanel {
private:
    Font poppinsFontBold;
    Vector2 virtualMouse;
    std::vector<PlaylistEntry>& playlists;
    int* rightClickedPlaylistIndex;
    int clickedIndex;

public:
    PlaylistGrid(Rectangle b, Font poppinsBold, Vector2 mouse, 
                 std::vector<PlaylistEntry>& p, int* rClickIdx);
    
    void Draw() override;
    int GetClickedIndex() const;
};

#endif // PLAYLISTGRID_H
