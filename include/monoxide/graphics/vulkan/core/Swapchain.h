#ifndef MONOXIDE_SWAPCHAIN_H
#define MONOXIDE_SWAPCHAIN_H

#include "monoxide/graphics/vulkan/core/Context.h"

namespace vulkan {
    class Swapchain;

    class SwapchainSupportDetails;
}

class vulkan::Swapchain {
public:
    Swapchain(std::shared_ptr<Context> context, uint32_t preferredDimensions[2]);

    ~Swapchain();

    void rebuild(uint32_t preferredDimensions[2]);

    vk::SwapchainKHR swapchain;

    std::vector<vk::Image> images;

    std::vector<vk::ImageView> imageViews;

    vk::SurfaceCapabilitiesKHR surfaceCapabilities;
    vk::SurfaceFormatKHR format;
    vk::PresentModeKHR presentMode;
    vk::Extent2D extent;
    uint32_t imageCount;

private:
    std::shared_ptr<Context> context;

};

class vulkan::SwapchainSupportDetails {
public:
    SwapchainSupportDetails(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);

    vk::SurfaceFormatKHR getOptimalSurfaceFormat();

    vk::PresentModeKHR getOptimalPresentMode();

    vk::Extent2D getOptimalExtent(uint32_t preferredDimensions[2]);

    uint32_t getOptimalImageCount();

    vk::SurfaceCapabilitiesKHR getCapabilities();

    std::vector<vk::SurfaceFormatKHR> getSurfaceFormats();

    std::vector<vk::PresentModeKHR> getPresentModes();

private:
    vk::SurfaceCapabilitiesKHR capabilities;

    std::vector<vk::SurfaceFormatKHR> formats;

    std::vector<vk::PresentModeKHR> presentModes;

};


#endif //MONOXIDE_SWAPCHAIN_H
