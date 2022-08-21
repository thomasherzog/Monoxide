#include "monoxide/graphics/vulkan/core/PhysicalDevice.h"

namespace vulkan::PhysicalDevice {

    std::vector<vk::PhysicalDevice>
    enumerateSupportedDevices(vk::Instance instance,
                              const std::vector<std::tuple<std::string, bool>> &requiredExtensions) {
        auto physicalDevices = instance.enumeratePhysicalDevices();

        physicalDevices.erase(
                std::remove_if(physicalDevices.begin(), physicalDevices.end(),
                               [&requiredExtensions](vk::PhysicalDevice physicalDevice) {
                                   for (auto tuple : requiredExtensions) {
                                       bool available = isExtensionAvailable(physicalDevice, std::get<0>(tuple));
                                       if (!std::get<1>(tuple) && !available) {
                                           return true;
                                       }
                                   }
                                   return false;
                               }
                ), physicalDevices.end());

        return physicalDevices;
    }

    std::vector<vk::PhysicalDevice>
    enumerateSupportedDevices(vk::Instance instance,
                              const std::vector<std::tuple<std::string, bool>> &requiredExtensions,
                              vk::SurfaceKHR surface) {

        auto physicalDevices = enumerateSupportedDevices(instance, requiredExtensions);

        physicalDevices.erase(std::remove_if(physicalDevices.begin(), physicalDevices.end(),
                                             [&surface](vk::PhysicalDevice physicalDevice) {
                                                 auto queueFamilies = physicalDevice.getQueueFamilyProperties();

                                                 int index = 0;
                                                 for (auto queueFamily : queueFamilies) {
                                                     bool presentSupport = physicalDevice.getSurfaceSupportKHR(index,
                                                                                                               surface);
                                                     if (queueFamily.queueFlags & vk::QueueFlagBits::eGraphics
                                                         && presentSupport) {
                                                         return false;
                                                     }
                                                 }
                                                 return true;
                                             }
        ), physicalDevices.end());


        return physicalDevices;
    }

    bool isExtensionAvailable(vk::PhysicalDevice physicalDevice, std::string extensionName) {
        auto extensionProperties = physicalDevice.enumerateDeviceExtensionProperties();

        auto iterator = std::find_if(extensionProperties.begin(), extensionProperties.end(),
                                     [&extensionName](vk::ExtensionProperties property) {
                                         return extensionName == property.extensionName;
                                     }
        );
        return iterator != extensionProperties.end();
    }

}