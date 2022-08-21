#ifndef MONOXIDE_DEBUGMESSENGER_H
#define MONOXIDE_DEBUGMESSENGER_H

#include <vulkan/vulkan.hpp>


namespace vulkan {
    class DebugMessenger;
}

class vulkan::DebugMessenger {
public:
    explicit DebugMessenger(vk::Instance instance);

    void destroy(vk::Instance instance);

    static vk::DebugUtilsMessengerCreateInfoEXT getCreateInfo();

private:
    vk::DebugUtilsMessengerEXT debugMessenger{VK_NULL_HANDLE};

    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                        VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                        void *pUserData);
};



#endif //MONOXIDE_DEBUGMESSENGER_H
