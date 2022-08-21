#include "monoxide/windowing/BaseWindow.h"

namespace windowing {

    BaseWindow::BaseWindow(const std::string &title, const std::vector<std::tuple<int, int>> &windowHints, int width,
                           int height) {
        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, false);
        for (auto hint: windowHints) {
            glfwWindowHint(std::get<0>(hint), std::get<1>(hint));
        }
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    }

    void BaseWindow::renderWindow() {
        onWindowRender();
        if (firstRender) {
            glfwShowWindow(window);
            firstRender = false;
        }
    }

    GLFWwindow *BaseWindow::getWindow() {
        return window;
    }

}
