#ifndef CONFIRMDIALOG_H
#define CONFIRMDIALOG_H

#include "popup.h"
#include <string>

enum ConfirmResult {
    CONFIRM_NONE = 0,
    CONFIRM_YES,
    CONFIRM_NO
};

class ConfirmDialog : public Popup {
private:
    Font poppinsFont;
    Font poppinsFontBold;
    std::string message;
    Vector2 virtualMouse;
    
public:
    ConfirmDialog(Rectangle b, Font font, Font fontBold, const std::string& msg, Vector2 mouse);
    void Draw() override;
    ConfirmResult GetResult() const;
};

#endif // CONFIRMDIALOG_H
