#ifndef MONOXIDE_IMGUIRENDERER_H
#define MONOXIDE_IMGUIRENDERER_H

#include "monoxide/graphics/vulkan/core/Context.h"
#include "monoxide/graphics/vulkan/core/Swapchain.h"
#include "monoxide/graphics/vulkan/render/RecordedCommandBuffer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

class ImGuiRenderer {
public:
    ImGuiRenderer(std::shared_ptr<vulkan::Context> context, GLFWwindow* window, vulkan::Swapchain &swapchain);

    ~ImGuiRenderer();

    void declareUserInterface(std::function<void()> const& lambda);

    void onSwapchainRebuild(vulkan::Swapchain &swapchain);

    RecordedCommandBuffer recordCommandBuffer(vulkan::Swapchain &swapchain, int imageIndex);

    ImGuiContext *imguiContext;

private:
    std::shared_ptr<vulkan::Context> context;

    std::vector<vk::Framebuffer> framebuffers;

    vk::DescriptorPool descriptorPool;

    vk::RenderPass renderPass;

    std::vector<vk::CommandPool> commandPools;

    std::vector<vk::CommandBuffer> commandBuffers;

    void createRenderPass(vulkan::Swapchain &swapchain);

    void createCommandPools(int imagesInFlight);

    void createCommandBuffers(int imagesInFlight);
};


#endif //MONOXIDE_IMGUIRENDERER_H
