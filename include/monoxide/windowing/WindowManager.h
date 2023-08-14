#pragma once

#include <memory>
#include "monoxide/windowing/BaseWindow.h"

namespace windowing {
    class WindowManager;
}

class windowing::WindowManager {
public:
    ~WindowManager();

    void startManager();

    void stopManager();

    void loopManager();

    void addWindow(const std::shared_ptr<BaseWindow> &window);

    static std::shared_ptr<WindowManager> getInstance();

private:
    WindowManager();

    bool running{false};

    std::vector<std::shared_ptr<BaseWindow>> windows;

    static std::shared_ptr<WindowManager> instance;

};