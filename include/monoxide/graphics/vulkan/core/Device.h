#pragma once

#include "monoxide/graphics/vulkan/core/PhysicalDevice.h"

#include <optional>

namespace vulkan {
    class Device;

    struct QueueFamily;
}

class vulkan::Device {

public:
    Device(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface,
           const std::vector<std::tuple<std::string, bool>> &requiredExtensions,
           vk::PhysicalDeviceFeatures features,
           void* pNext);

    ~Device();

    std::optional<QueueFamily> findQueueFamily(vk::QueueFlagBits flags);

    vk::Device getVkDevice();

    vk::PhysicalDevice getPhysicalDevice();

    vk::Queue getGraphicsQueue();

    uint32_t getGraphicsQueueFamily();

    vk::Queue getPresentQueue();

    uint32_t getPresentQueueFamily();

    vk::Queue getComputeQueue();

    uint32_t getComputeQueueFamily();

    vk::Queue getTransferQueue();

    uint32_t getTransferQueueFamily();

private:
    vk::Device device;

    vk::PhysicalDevice physicalDevice;

    std::vector<QueueFamily> queueFamilies;

    std::tuple<vk::Queue, uint32_t> graphicsQueue{VK_NULL_HANDLE, 0};
    std::tuple<vk::Queue, uint32_t> presentQueue{VK_NULL_HANDLE, 0};
    std::tuple<vk::Queue, uint32_t> computeQueue{VK_NULL_HANDLE, 0};
    std::tuple<vk::Queue, uint32_t> transferQueue{VK_NULL_HANDLE, 0};

    static std::vector<QueueFamily> findQueueFamilies(vk::PhysicalDevice physicalDevice);

};

struct vulkan::QueueFamily {
    uint32_t index;
    vk::QueueFamilyProperties properties;
};