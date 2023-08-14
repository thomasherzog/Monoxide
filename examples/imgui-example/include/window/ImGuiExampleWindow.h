#pragma once

#include <monoxide/windowing/ImGuiWindow.h>
#include <monoxide/graphics/vulkan/memory/Texture.h>

class ImGuiExampleWindow : public windowing::ImGuiWindow {
public:
    ImGuiExampleWindow();

    ~ImGuiExampleWindow();

    void onRenderUI() override;

};