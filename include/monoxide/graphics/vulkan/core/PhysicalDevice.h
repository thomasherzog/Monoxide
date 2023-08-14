#pragma once

#include <vulkan/vulkan.hpp>

namespace vulkan::PhysicalDevice {

    std::vector<vk::PhysicalDevice>
    enumerateSupportedDevices(vk::Instance instance,
                              const std::vector<std::tuple<std::string, bool>> &requiredExtensions);

    std::vector<vk::PhysicalDevice>
    enumerateSupportedDevices(vk::Instance instance,
                              const std::vector<std::tuple<std::string, bool>> &requiredExtensions,
                              vk::SurfaceKHR surface);

    bool isExtensionAvailable(vk::PhysicalDevice physicalDevice, std::string extensionName);

}