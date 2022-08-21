#ifndef MONOXIDE_SURFACE_H
#define MONOXIDE_SURFACE_H

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>


namespace vulkan {
    class Surface;
}

class vulkan::Surface {
public:
    Surface(vk::Instance instance, GLFWwindow *window);

    void destroy(vk::Instance instance);

    vk::SurfaceKHR getSurface();

private:
    vk::SurfaceKHR surface;

};


#endif //MONOXIDE_SURFACE_H
