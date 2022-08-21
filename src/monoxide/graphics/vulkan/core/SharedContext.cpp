#include "monoxide/graphics/vulkan/core/SharedContext.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.h>


namespace vulkan {

    SharedContext::SharedContext(const std::vector<std::tuple<std::string, bool>> &instanceExtensions,
                                 const std::vector<std::string> &validationLayers,
                                 const std::vector<std::tuple<std::string, bool>> &deviceExtensions,
                                 vk::PhysicalDeviceFeatures physicalDeviceFeatures,
                                 void* devicePNext,
                                 GLFWwindow *window) {

        vk::DynamicLoader dynamicLoader;
        auto vkGetInstanceProcAddr = dynamicLoader.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
        VULKAN_HPP_DEFAULT_DISPATCHER.init(vkGetInstanceProcAddr);

        instance = std::make_shared<Instance>(instanceExtensions, validationLayers);

        surface = std::make_shared<Surface>(instance->getInstance(), window);

        auto physicalDevice = PhysicalDevice::enumerateSupportedDevices(instance->getInstance(),
                                                                        deviceExtensions);

        device = std::make_shared<Device>(physicalDevice[0], surface->getSurface(), deviceExtensions, physicalDeviceFeatures, devicePNext);
        VULKAN_HPP_DEFAULT_DISPATCHER.init(device->getVkDevice());

        VmaAllocatorCreateInfo allocatorCreateInfo{};
        allocatorCreateInfo.instance = instance->getInstance();
        allocatorCreateInfo.physicalDevice = device->getPhysicalDevice();
        allocatorCreateInfo.device = device->getVkDevice();
        vmaCreateAllocator(&allocatorCreateInfo, &allocator);
    }

    SharedContext::~SharedContext() {
        vmaDestroyAllocator(allocator);
        device.reset();
        surface->destroy(instance->getInstance());
        instance.reset();
    }

    std::shared_ptr<Instance> SharedContext::getInstance() {
        return instance;
    }

    std::shared_ptr<Surface> SharedContext::getSurface() {
        return surface;
    }

    std::shared_ptr<Device> SharedContext::getDevice() {
        return device;
    }

    VmaAllocator SharedContext::getAllocator() {
        return allocator;
    }

}