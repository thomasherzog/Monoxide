#include "monoxide/graphics/vulkan/core/Context.h"

namespace vulkan {

    Context::Context(const std::vector<std::tuple<std::string, bool>> &instanceExtensions,
                     const std::vector<std::string> &validationLayers,
                     const std::vector<std::tuple<std::string, bool>> &deviceExtensions,
                     vk::PhysicalDeviceFeatures physicalDeviceFeatures,
                     void* devicePNext, GLFWwindow *window) {
        sharedContext = std::make_shared<SharedContext>(instanceExtensions, validationLayers, deviceExtensions, physicalDeviceFeatures, devicePNext, window);

        auto queueFamily = sharedContext->getDevice()->findQueueFamily(vk::QueueFlagBits::eGraphics);
        if (queueFamily.has_value()) {
            vk::CommandPoolCreateInfo createInfo(vk::CommandPoolCreateFlagBits::eTransient, queueFamily->index);
            transientCommandPool = sharedContext->getDevice()->getVkDevice().createCommandPool(createInfo);
        }
    }

    Context::Context(std::shared_ptr<SharedContext> sharedContext) : sharedContext(sharedContext) {
        auto queueFamily = sharedContext->getDevice()->findQueueFamily(vk::QueueFlagBits::eGraphics);
        if (queueFamily.has_value()) {
            vk::CommandPoolCreateInfo createInfo(vk::CommandPoolCreateFlagBits::eTransient, queueFamily->index);
            transientCommandPool = sharedContext->getDevice()->getVkDevice().createCommandPool(createInfo);
        }
    }

    Context::~Context() {
        sharedContext->getDevice()->getVkDevice().destroyCommandPool(transientCommandPool);
    }

    vk::CommandBuffer Context::beginTransientExecution() {
        vk::CommandBufferAllocateInfo allocateInfo(transientCommandPool, vk::CommandBufferLevel::ePrimary, 1);
        auto commandBuffer = sharedContext->getDevice()->getVkDevice().allocateCommandBuffers(allocateInfo)[0];
        commandBuffer.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));
        return commandBuffer;
    }

    void Context::endTransientExecution(vk::CommandBuffer commandBuffer) {
        commandBuffer.end();
        vk::SubmitInfo submitInfo(0, nullptr, nullptr, 1, &commandBuffer, 0, nullptr);
        sharedContext->getDevice()->getGraphicsQueue().submit(submitInfo, nullptr);

        sharedContext->getDevice()->getGraphicsQueue().waitIdle();

        sharedContext->getDevice()->getVkDevice().freeCommandBuffers(transientCommandPool, 1, &commandBuffer);
    }

    std::shared_ptr<Instance> Context::getInstance() {
        return sharedContext->getInstance();
    }

    std::shared_ptr<Surface> Context::getSurface() {
        return sharedContext->getSurface();
    }

    std::shared_ptr<Device> Context::getDevice() {
        return sharedContext->getDevice();
    }

    VmaAllocator Context::getAllocator() {
        return sharedContext->getAllocator();
    }

}