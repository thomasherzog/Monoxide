#pragma once

#include "monoxide/windowing/VulkanWindow.h"
#include "monoxide/renderer/ImGuiRenderer.h"

namespace windowing {
    class ImGuiWindow;
}

class windowing::ImGuiWindow : public windowing::VulkanWindow {
public:
    ImGuiWindow();

    ImGuiWindow(const std::string& title, int width, int height);

    ~ImGuiWindow();

    void onRender(vulkan::SyncObject syncObject, uint32_t imageIndex) final;

    void onSwapchainRebuild() override;

    virtual void onPreUIRender();

    virtual void onPostUIRender();

    virtual void onRenderUI() = 0;

private:
    std::unique_ptr<ImGuiRenderer> imguiRenderer;
};