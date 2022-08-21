#include "monoxide/windowing/WindowManager.h"
#include "window/ImGuiExampleWindow.h"

int main() {
    auto manager = windowing::WindowManager::getInstance();
    manager->addWindow(std::make_shared<ImGuiExampleWindow>());
    manager->startManager();

    return 0;
}