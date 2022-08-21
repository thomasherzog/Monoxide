#include "monoxide/windowing/ImGuiWindow.h"

namespace windowing {

    ImGuiWindow::ImGuiWindow() {
        imguiRenderer = std::make_unique<ImGuiRenderer>(context, window, *swapchain);
    }

    ImGuiWindow::ImGuiWindow(const std::string &title, int width, int height) : VulkanWindow(title, width, height) {
        imguiRenderer = std::make_unique<ImGuiRenderer>(context, window, *swapchain);
    }

    ImGuiWindow::~ImGuiWindow() {
        context->getDevice()->getVkDevice().waitIdle();
    }

    void ImGuiWindow::onRender(vulkan::SyncObject syncObject, uint32_t imageIndex) {
        context->getDevice()->getVkDevice().resetFences(syncObject.fence);

        imguiRenderer->declareUserInterface([this]() { this->onRenderUI(); });
        std::vector<vk::PipelineStageFlags> pipelineStageFlags = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
        std::vector<vk::Semaphore> semaphores = {syncObject.imageAvailableSemaphore};
        auto imguiRendererCmd = imguiRenderer->recordCommandBuffer(*swapchain, imageIndex);
        imguiRendererCmd.submit(
                semaphores,
                pipelineStageFlags,
                {syncObject.renderFinishedSemaphore},
                syncObject.fence
        );
    }

    void ImGuiWindow::onSwapchainRebuild() {
        imguiRenderer->onSwapchainRebuild(*swapchain);
    }

    void ImGuiWindow::onPreUIRender() {

    }

    void ImGuiWindow::onPostUIRender() {

    }


}