#include "monoxide/graphics/vulkan/core/Surface.h"

namespace vulkan {

    Surface::Surface(vk::Instance instance, GLFWwindow *window) {
        glfwCreateWindowSurface(instance, window, nullptr, reinterpret_cast<VkSurfaceKHR *>(&surface));
    }

    void Surface::destroy(vk::Instance instance) {
        instance.destroySurfaceKHR(surface);
    }

    vk::SurfaceKHR Surface::getSurface() {
        return surface;
    }

}