#include "monoxide/windowing/VulkanWindow.h"

namespace windowing {

    VulkanWindow::VulkanWindow() : VulkanWindow("Vulkan Window - Monoxide", 1920, 1080) {}

    VulkanWindow::VulkanWindow(const std::string &title, int width, int height) : BaseWindow(title,
                                                                                             getRequiredWindowHints(),
                                                                                             width, height) {
        if (!glfwVulkanSupported()) {
            throw std::runtime_error("Vulkan not supported!");
        }

        std::vector<std::tuple<std::string, bool>> instanceExtensions = {
                {"OOF", true} // Optional extension
        };
        const std::vector<std::string> validationLayers = {
                "VK_LAYER_KHRONOS_validation",
        };
        std::vector<std::tuple<std::string, bool>> deviceExtensions = {
                {VK_KHR_SHADER_CLOCK_EXTENSION_NAME, false},
                {VK_KHR_SWAPCHAIN_EXTENSION_NAME,    false},
        };

        uint32_t extensions_count = 0;
        const char **extensions = glfwGetRequiredInstanceExtensions(&extensions_count);
        std::vector<const char *> glfwExtensions(extensions, extensions + extensions_count);
        for (auto ex: glfwExtensions) {
            instanceExtensions.emplace_back(ex, false);
        }

        vk::PhysicalDeviceFeatures features{};
        features.shaderInt64 = true;
        features.shaderFloat64 = true;

        vk::PhysicalDeviceShaderClockFeaturesKHR shaderClockFeatures{true};

        context = std::make_shared<vulkan::Context>(instanceExtensions, validationLayers, deviceExtensions, features,
                                                    &shaderClockFeatures, window);

        int gWidth, gHeight;
        glfwGetWindowSize(window, &gWidth, &gHeight);
        uint32_t windowSize[2] = {static_cast<uint32_t>(gWidth), static_cast<uint32_t>(gHeight)};
        swapchain = std::make_unique<vulkan::Swapchain>(context, windowSize);

        inFlightFrames = std::make_unique<vulkan::InFlightFrames>(context, 2);

        imagesInFlight.resize(swapchain->imageCount, VK_NULL_HANDLE);

        for (int i = 0; i < swapchain->imageCount; ++i) {
            commandPools.emplace_back(
                    context->getDevice()->getVkDevice().createCommandPool(
                            vk::CommandPoolCreateInfo({}, context->getDevice()->getGraphicsQueueFamily())));
        }

        commandBuffers.resize(swapchain->imageCount);
        for (int i = 0; i < swapchain->imageCount; ++i) {
            commandBuffers[i] = context->getDevice()->getVkDevice().allocateCommandBuffers(
                    vk::CommandBufferAllocateInfo(commandPools[i], vk::CommandBufferLevel::ePrimary, 1))[0];
        }

        glfwSetWindowUserPointer(window, this);
    }

    VulkanWindow::~VulkanWindow() {
        context->getDevice()->getVkDevice().waitIdle();

        for (auto &commandPool: commandPools) {
            context->getDevice()->getVkDevice().destroyCommandPool(commandPool);
        }
    }

    std::optional<uint32_t> VulkanWindow::acquireNextImage(vulkan::SyncObject syncObject) {
        context->getDevice()->getVkDevice().waitForFences(syncObject.fence, true, UINT64_MAX);

        vk::ResultValue<uint32_t> result{vk::Result::eNotReady, uint32_t{0}};
        try {
            result = context->getDevice()->getVkDevice().acquireNextImageKHR(swapchain->swapchain,
                                                                             UINT64_MAX,
                                                                             syncObject.imageAvailableSemaphore,
                                                                             nullptr);
        } catch (vk::OutOfDateKHRError &) {
            result = vk::ResultValue<uint32_t>{vk::Result::eErrorOutOfDateKHR, uint32_t{0}};
        }
        switch (result.result) {
            case vk::Result::eSuccess:
            case vk::Result::eSuboptimalKHR:
                break;
            case vk::Result::eErrorOutOfDateKHR:
                recreateSwapchain();
                return std::nullopt;
            default:
                throw std::runtime_error("Swap chain image acquisition failed");
        }
        auto imageIndex = result.value;

        if (imagesInFlight[imageIndex]) {
            context->getDevice()->getVkDevice().waitForFences(imagesInFlight[imageIndex], true, UINT64_MAX);
        }

        imagesInFlight[imageIndex] = syncObject.fence;
        return imageIndex;
    }

    void VulkanWindow::presentFrame(vulkan::SyncObject syncObject, uint32_t imageIndex) {
        vk::PresentInfoKHR presentInfo(1, &syncObject.renderFinishedSemaphore,
                                       1, &swapchain->swapchain,
                                       &imageIndex);

        vk::Result result;
        try {
            result = context->getDevice()->getPresentQueue().presentKHR(presentInfo);
        } catch (vk::OutOfDateKHRError &) {
            result = vk::Result::eErrorOutOfDateKHR;
        }

        switch (result) {
            case vk::Result::eSuccess:
            case vk::Result::eSuboptimalKHR:
                break;
            case vk::Result::eErrorOutOfDateKHR:
                recreateSwapchain();
                break;
            default:
                throw std::runtime_error("Swap chain image presentation failed");
        }
    }

    void VulkanWindow::recreateSwapchain() {
        int width = 0, height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        if (width != 0 && height != 0) {
            context->getDevice()->getVkDevice().waitIdle();
            uint32_t dim[2] = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
            swapchain->rebuild(dim);
            imagesInFlight.resize(swapchain->imageCount, nullptr);
            onSwapchainRebuild();
        }
    }

    void VulkanWindow::onWindowRender() {
        auto syncObject = inFlightFrames->getNextSyncObject();
        std::optional<uint32_t> imageIndex = acquireNextImage(syncObject);
        if (imageIndex.has_value()) {
            onRender(syncObject, imageIndex.value());
            presentFrame(syncObject, imageIndex.value());
        }
    }

    std::vector<std::tuple<int, int>> VulkanWindow::getRequiredWindowHints() {
        std::vector<std::tuple<int, int>> hints;
        hints.emplace_back(GLFW_CLIENT_API, GLFW_NO_API);
        hints.emplace_back(GLFW_SRGB_CAPABLE, GLFW_TRUE);
        return hints;
    }

}
