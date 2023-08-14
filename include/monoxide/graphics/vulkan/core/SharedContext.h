#pragma once

#include "monoxide/graphics/vulkan/core/Instance.h"
#include "monoxide/graphics/vulkan/core/Surface.h"
#include "monoxide/graphics/vulkan/core/Device.h"
#include "monoxide/graphics/vulkan/debug/DebugMessenger.h"

#include <vk_mem_alloc.h>

namespace vulkan {
    class SharedContext;
}

class vulkan::SharedContext {
public:
    SharedContext(const std::vector<std::tuple<std::string, bool>> &instanceExtensions,
                  const std::vector<std::string> &validationLayers,
                  const std::vector<std::tuple<std::string, bool>>& deviceExtensions,
                  vk::PhysicalDeviceFeatures physicalDeviceFeatures,
                  void* devicePNext,
                  GLFWwindow *window);

    ~SharedContext();

    std::shared_ptr<Instance> getInstance();

    std::shared_ptr<Surface> getSurface();

    std::shared_ptr<Device> getDevice();

    VmaAllocator getAllocator();

private:
    std::shared_ptr<Instance> instance;

    std::shared_ptr<Surface> surface;

    std::shared_ptr<Device> device;

    VmaAllocator allocator{};

};