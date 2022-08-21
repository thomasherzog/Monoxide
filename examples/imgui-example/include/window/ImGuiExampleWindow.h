#ifndef MONOXIDE_IMGUIEXAMPLEWINDOW_H
#define MONOXIDE_IMGUIEXAMPLEWINDOW_H

#include <monoxide/windowing/ImGuiWindow.h>

class ImGuiExampleWindow : public windowing::ImGuiWindow {
public:
    ImGuiExampleWindow();

    ~ImGuiExampleWindow();

    void onRenderUI() override;

private:

};


#endif //MONOXIDE_IMGUIEXAMPLEWINDOW_H
