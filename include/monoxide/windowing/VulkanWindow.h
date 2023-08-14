#pragma once

#include "monoxide/windowing/BaseWindow.h"
#include "monoxide/graphics/vulkan/core/Swapchain.h"
#include "monoxide/graphics/vulkan/core/InFlightFrames.h"

namespace windowing {
    class VulkanWindow;
}

class windowing::VulkanWindow : public BaseWindow {
public:
    VulkanWindow();

    VulkanWindow(const std::string& title, int width, int height);

    ~VulkanWindow();

    virtual void onRender(vulkan::SyncObject syncObject, uint32_t imageIndex) = 0;

    virtual void onSwapchainRebuild() = 0;

    std::optional<uint32_t> acquireNextImage(vulkan::SyncObject syncObject);

    void presentFrame(vulkan::SyncObject syncObject, uint32_t imageIndex);

    void recreateSwapchain();

    void onWindowRender() override;

protected:
    std::shared_ptr<vulkan::Context> context;

    std::unique_ptr<vulkan::Swapchain> swapchain;

    std::vector<vk::CommandPool> commandPools;

    std::vector<vk::CommandBuffer> commandBuffers;

    std::unique_ptr<vulkan::InFlightFrames> inFlightFrames;

    std::vector<vk::Fence> imagesInFlight;

    //FunctionQueue preRenderQueue;

private:
    static std::vector<std::tuple<int, int>> getRequiredWindowHints();

};