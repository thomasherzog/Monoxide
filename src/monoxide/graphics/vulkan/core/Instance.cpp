#include "monoxide/graphics/vulkan/core/Instance.h"

#include <iostream>

namespace vulkan {

    Instance::Instance(std::vector<std::tuple<std::string, bool>> requiredExtensions,
                       std::vector<std::string> validationLayers) {

        vk::ApplicationInfo applicationInfo("Monoxide", 1, "Monoxide Engine", 1, VK_API_VERSION_1_2);

        vk::InstanceCreateInfo instanceCreateInfo({}, &applicationInfo);

        bool enableValidation = false;
        if (!validationLayers.empty() && isExtensionAvailable("VK_EXT_debug_utils")) {
            auto iterator = std::find_if(requiredExtensions.begin(), requiredExtensions.end(),
                                         [](std::tuple<std::string, bool> tuple) {
                                             return std::get<0>(tuple) == "VK_EXT_debug_utils";
                                         });

            if (iterator == requiredExtensions.end()) {
                requiredExtensions.emplace_back("VK_EXT_debug_utils", true);
            }
            enableValidation = true;
        }

        std::vector<char const *> layers;
        if (enableValidation) {
            validationLayers.erase(std::remove_if(validationLayers.begin(), validationLayers.end(),
                                                  [](const std::string &layer) {
                                                      if (!isLayerAvailable(layer)) {
                                                          std::cout << "[WARNING] Layer " << layer << " not available"
                                                                    << std::endl;
                                                          return true;
                                                      }
                                                      return false;
                                                  }), validationLayers.end());

            std::transform(validationLayers.begin(), validationLayers.end(), std::back_inserter(layers),
                           [](const std::string &layer) {
                               return layer.c_str();
                           });
        }

        instanceCreateInfo.enabledLayerCount = layers.size();
        instanceCreateInfo.ppEnabledLayerNames = layers.data();

        std::vector<const char *> extensions;
        for (auto &tuple : requiredExtensions) {
            bool available = isExtensionAvailable(std::get<0>(tuple));
            if (!std::get<1>(tuple) && !available) {
                std::string buffer("[ERROR] Instance extension ");
                buffer.append(std::get<0>(tuple));
                buffer.append(" not present");
                throw std::runtime_error(buffer);
            }
            if (available) {
                extensions.push_back(std::get<0>(tuple).data());
            }
        }

        instanceCreateInfo.enabledExtensionCount = extensions.size();
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

        if (!layers.empty()) {
            auto debugCreateInfo = DebugMessenger::getCreateInfo();
            instanceCreateInfo.pNext = &debugCreateInfo;
        }
        instance = vk::createInstance(instanceCreateInfo);
        VULKAN_HPP_DEFAULT_DISPATCHER.init(instance);

        if(!layers.empty()) {
            debugMessenger = DebugMessenger(instance);
        }
    }

    Instance::~Instance() {
        if (debugMessenger.has_value()) {
            debugMessenger->destroy(instance);
            debugMessenger.reset();
        }
        instance.destroy();
    }

    bool Instance::isExtensionAvailable(std::string extensionName) {
        auto extensionProperties = vk::enumerateInstanceExtensionProperties();

        auto iterator = std::find_if(extensionProperties.begin(), extensionProperties.end(),
                                     [&extensionName](vk::ExtensionProperties const &property) {
                                         return extensionName == property.extensionName;
                                     });

        return iterator != extensionProperties.end();
    }

    bool Instance::isLayerAvailable(std::string layerName) {
        auto layerProperties = vk::enumerateInstanceLayerProperties();
        auto iterator = std::find_if(layerProperties.begin(), layerProperties.end(),
                                     [&layerName](vk::LayerProperties const &property) {
                                         return layerName == property.layerName;
                                     });
        return iterator != layerProperties.end();
    }

    vk::Instance Instance::getInstance() {
        return instance;
    }

}