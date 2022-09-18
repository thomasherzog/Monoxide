#include "window/ImGuiExampleWindow.h"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <iostream>

ImGuiExampleWindow::ImGuiExampleWindow() : windowing::ImGuiWindow("ImGui Example Window", 1200, 600) {

    int texWidth, texHeight, texChannels;
    stbi_uc *pixels = stbi_load("./testimg.png", &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    if (!pixels) {
        std::cout << "Failed to load texture file" << std::endl;
        return;
    }

    texture = std::make_unique<vulkan::Texture>(context, texWidth, texHeight, pixels);
    textureId = ImGui_ImplVulkan_AddTexture(texture->sampler, texture->imageView,
                                            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

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

    ImGui::GetWindowDrawList()->AddRectFilled({428, 428}, {502, 502}, ImColor{0, 0, 0}, 3.0f);
    ImGui::GetWindowDrawList()->AddRectFilled({430, 430}, {500, 500}, ImColor{255, 255, 255}, 3.0f);

    ImGui::GetWindowDrawList()->AddCircleFilled(ImGui::GetMousePos(), 12.0f, ImColor{0, 0, 0});
    ImGui::GetWindowDrawList()->AddCircleFilled(ImGui::GetMousePos(), 10.0f, ImColor{255, 255, 255});

    ImGui::Image(textureId, {200, 200});

    ImGui::End();

    ImGui::ShowDemoWindow();
}
