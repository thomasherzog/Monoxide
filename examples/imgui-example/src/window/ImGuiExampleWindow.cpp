#include "window/ImGuiExampleWindow.h"

ImGuiExampleWindow::ImGuiExampleWindow() : windowing::ImGuiWindow("ImGui Example Window", 1200, 600) {}

ImGuiExampleWindow::~ImGuiExampleWindow() { context->getDevice()->getVkDevice().waitIdle(); };

void ImGuiExampleWindow::onRenderUI() {

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    auto flags = ImGuiWindowFlags_NoResize
                 | ImGuiWindowFlags_NoDecoration
                 | ImGuiWindowFlags_NoMove
                 | ImGuiWindowFlags_NoCollapse;

    ImGui::Begin("Test Window", nullptr, flags);
    ImGui::Text("Hello World!");
    ImGui::Button("Hello button");

    ImGui::End();

    ImGui::ShowDemoWindow();
}
