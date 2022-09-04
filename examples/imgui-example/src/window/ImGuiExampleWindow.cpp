#include "window/ImGuiExampleWindow.h"

ImGuiExampleWindow::ImGuiExampleWindow() : windowing::ImGuiWindow("ImGui Example Window", 1200, 600) {}

ImGuiExampleWindow::~ImGuiExampleWindow() = default;

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

    ImGui::GetWindowDrawList()->AddRectFilled({428, 428}, {502, 502}, ImColor{0, 0, 0}, 3.0f);
    ImGui::GetWindowDrawList()->AddRectFilled({430, 430}, {500, 500}, ImColor{255, 255, 255}, 3.0f);

    ImGui::GetWindowDrawList()->AddCircleFilled(ImGui::GetMousePos(), 12.0f, ImColor{0,0,0});
    ImGui::GetWindowDrawList()->AddCircleFilled(ImGui::GetMousePos(), 10.0f, ImColor{255, 255, 255});

    ImGui::End();

    ImGui::ShowDemoWindow();
}
