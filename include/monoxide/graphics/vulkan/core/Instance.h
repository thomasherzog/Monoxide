#pragma once

#include "monoxide/graphics/vulkan/debug/DebugMessenger.h"

#include <optional>

namespace vulkan {
    class Instance;
}

class vulkan::Instance {
public:
    Instance(std::vector<std::tuple<std::string, bool>> requiredExtensions,
             std::vector<std::string> validationLayers);

    ~Instance();

    vk::Instance getInstance();

    static bool isExtensionAvailable(std::string extensionName);

    static bool isLayerAvailable(std::string layerName);

private:
    vk::Instance instance{VK_NULL_HANDLE};

    std::optional<DebugMessenger> debugMessenger;
};