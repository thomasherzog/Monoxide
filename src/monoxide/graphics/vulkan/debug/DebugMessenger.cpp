#include "monoxide/graphics/vulkan/debug/DebugMessenger.h"

#include <iostream>

namespace vulkan {

    DebugMessenger::DebugMessenger(vk::Instance instance) {
        debugMessenger = instance.createDebugUtilsMessengerEXT(getCreateInfo());
    }

    void DebugMessenger::destroy(vk::Instance instance) {
        if (debugMessenger) {
            instance.destroyDebugUtilsMessengerEXT(debugMessenger);
        }
    }

    vk::DebugUtilsMessengerCreateInfoEXT DebugMessenger::getCreateInfo() {
        vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
                vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
                | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError
        );

        vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(
                vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
                | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
                | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
        );

        return {{}, severityFlags, messageTypeFlags, &debugCallback};
    }

    VkBool32 DebugMessenger::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                           VkDebugUtilsMessageTypeFlagsEXT messageType,
                                           const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData, void *pUserData) {
        std::ostringstream message;

        message << vk::to_string(static_cast<vk::DebugUtilsMessageSeverityFlagBitsEXT>( messageSeverity )) << ": "
                << vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>( messageType )) << ":\n";
        message << "\t" << "Message ID Name   : " << pCallbackData->pMessageIdName << "\n";
        message << "\t" << "Message ID Number : " << pCallbackData->messageIdNumber << "\n";
        message << "\t" << "Message           : " << pCallbackData->pMessage << "\n";

        if (0 < pCallbackData->queueLabelCount) {
            message << "\t" << "Queue Labels:\n";
            for (size_t i = 0; i < pCallbackData->queueLabelCount; i++) {
                message << "\t\t" << "labelName = <" << pCallbackData->pQueueLabels[i].pLabelName << ">\n";
            }
        }
        if (0 < pCallbackData->cmdBufLabelCount) {
            message << "\t" << "CommandBuffer Labels:\n";
            for (size_t i = 0; i < pCallbackData->cmdBufLabelCount; i++) {
                message << "\t\t" << "labelName = <" << pCallbackData->pCmdBufLabels[i].pLabelName << ">\n";
            }
        }
        if (0 < pCallbackData->objectCount) {
            message << "\t" << "Objects:\n";
            for (size_t i = 0; i < pCallbackData->objectCount; i++) {
                message << "\t\t" << "Object " << i << ": " << "\n";
                message << "\t\t\t" << "objectType   = "
                        << vk::to_string(static_cast<vk::ObjectType>( pCallbackData->pObjects[i].objectType )) << "\n";
                message << "\t\t\t" << "objectHandle = " << pCallbackData->pObjects[i].objectHandle << "\n";
                if (pCallbackData->pObjects[i].pObjectName) {
                    message << "\t\t\t" << "objectName   = <" << pCallbackData->pObjects[i].pObjectName << ">\n";
                }
            }
        }

        std::cout << message.str() << std::endl;
        return false;
    }

}