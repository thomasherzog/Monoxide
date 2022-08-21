#ifndef MONOXIDE_BASEWINDOW_H
#define MONOXIDE_BASEWINDOW_H

#include <vector>
#include <string>
#include <GLFW/glfw3.h>

namespace windowing {
    class BaseWindow;
}

class windowing::BaseWindow {
public:
    BaseWindow(const std::string &title, const std::vector<std::tuple<int, int>> &windowHints,
               int width, int height);

    void renderWindow();

    GLFWwindow *getWindow();

protected:
    GLFWwindow *window;

    bool firstRender{true};

    virtual void onWindowRender() = 0;
};


#endif //MONOXIDE_BASEWINDOW_H
