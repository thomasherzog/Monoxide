#include "monoxide/graphics/vulkan/core/Device.h"

namespace vulkan {

    Device::Device(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface,
                   const std::vector<std::tuple<std::string, bool>> &requiredExtensions,
                   vk::PhysicalDeviceFeatures features,
                   void* pNext) : physicalDevice(physicalDevice) {

        std::vector<const char *> extensions;
        for (auto &tuple : requiredExtensions) {
            bool available = PhysicalDevice::isExtensionAvailable(physicalDevice, std::get<0>(tuple));
            if (!std::get<1>(tuple) && !available) {
                std::string buffer("[ERROR] Device extension ");
                buffer.append(std::get<0>(tuple));
                buffer.append(" not present");
                throw std::runtime_error(buffer);
            }
            if (available) {
                extensions.push_back(std::get<0>(tuple).data());
            }
        }

        queueFamilies = findQueueFamilies(physicalDevice);

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        float queuePriority = 1.0f;
        for (auto index : queueFamilies) {
            vk::DeviceQueueCreateInfo queueCreateInfo({}, index.index, 1, &queuePriority);
            queueCreateInfos.push_back(queueCreateInfo);
        }

        vk::DeviceCreateInfo createInfo({}, queueCreateInfos, nullptr, extensions, &features);
        createInfo.pNext = pNext;

        device = physicalDevice.createDevice(createInfo);

        for (auto index : queueFamilies) {
            if (!std::get<0>(graphicsQueue) && index.properties.queueFlags & vk::QueueFlagBits::eGraphics) {
                std::get<0>(graphicsQueue) = device.getQueue(index.index, 0);
                std::get<1>(graphicsQueue) = index.index;
            } else if (!std::get<0>(computeQueue) && index.properties.queueFlags & vk::QueueFlagBits::eCompute) {
                std::get<0>(computeQueue) = device.getQueue(index.index, 0);
                std::get<1>(computeQueue) = index.index;
            } else if (!std::get<0>(transferQueue) && index.properties.queueFlags & vk::QueueFlagBits::eTransfer) {
                std::get<0>(transferQueue) = device.getQueue(index.index, 0);
                std::get<1>(transferQueue) = index.index;
            }
        }

        for (auto index : queueFamilies) {
            if (physicalDevice.getSurfaceSupportKHR(index.index, surface)) {
                std::get<0>(presentQueue) = device.getQueue(index.index, 0);
                std::get<1>(presentQueue) = index.index;
                break;
            }
        }
    }

    Device::~Device() {
        device.destroy();
    }

    std::vector<QueueFamily> Device::findQueueFamilies(vk::PhysicalDevice physicalDevice) {
        std::vector<vk::QueueFamilyProperties> queueFamilies = physicalDevice.getQueueFamilyProperties();

        std::vector<QueueFamily> indices;
        uint32_t i = 0;
        for (const auto &queueFamily : queueFamilies) {
            QueueFamily index{i, queueFamily};
            indices.push_back(index);
            i++;
        }

        return indices;
    }

    std::optional<QueueFamily> Device::findQueueFamily(vk::QueueFlagBits flags) {
        for (auto queueFamily : queueFamilies) {
            if (queueFamily.properties.queueFlags & flags) {
                return queueFamily;
            }
        }
        return {};
    }

    vk::Device Device::getVkDevice() {
        return device;
    }

    vk::PhysicalDevice Device::getPhysicalDevice() {
        return physicalDevice;
    }

    vk::Queue Device::getGraphicsQueue() {
        return std::get<0>(graphicsQueue);
    }

    uint32_t Device::getGraphicsQueueFamily() {
        return std::get<1>(graphicsQueue);
    }

    vk::Queue Device::getPresentQueue() {
        return std::get<0>(presentQueue);
    }

    uint32_t Device::getPresentQueueFamily() {
        return std::get<1>(presentQueue);
    }

    vk::Queue Device::getComputeQueue() {
        return std::get<0>(computeQueue);
    }

    uint32_t Device::getComputeQueueFamily() {
        return std::get<1>(computeQueue);
    }

    vk::Queue Device::getTransferQueue() {
        return std::get<0>(transferQueue);
    }

    uint32_t Device::getTransferQueueFamily() {
        return std::get<1>(transferQueue);
    }

}