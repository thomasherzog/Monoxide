#include "monoxide/graphics/vulkan/core/Swapchain.h"

namespace vulkan {

    Swapchain::Swapchain(std::shared_ptr<Context> context, uint32_t *preferredDimensions) : context(context) {
        SwapchainSupportDetails details{context->getDevice()->getPhysicalDevice(),
                                        context->getSurface()->getSurface()};

        surfaceCapabilities = details.getCapabilities();
        format = details.getOptimalSurfaceFormat();
        presentMode = details.getOptimalPresentMode();
        extent = details.getOptimalExtent(preferredDimensions);
        imageCount = details.getOptimalImageCount();

        vk::SwapchainCreateInfoKHR createInfo({}, context->getSurface()->getSurface(),
                                              imageCount,
                                              format.format,
                                              format.colorSpace,
                                              extent,
                                              1,
                                              vk::ImageUsageFlagBits::eColorAttachment);

        if (context->getDevice()->getGraphicsQueueFamily() != context->getDevice()->getPresentQueueFamily()) {
            uint32_t familyIndices[2] = {context->getDevice()->getGraphicsQueueFamily(),
                                         context->getDevice()->getPresentQueueFamily()};
            createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = familyIndices;
        } else {
            createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        }

        createInfo.preTransform = surfaceCapabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = presentMode;
        createInfo.clipped = true;
        createInfo.oldSwapchain = nullptr;

        swapchain = context->getDevice()->getVkDevice().createSwapchainKHR(createInfo);

        images = context->getDevice()->getVkDevice().getSwapchainImagesKHR(swapchain);

        imageViews.resize(images.size());
        for (size_t i = 0; i < imageViews.size(); i++) {
            vk::ImageViewCreateInfo imageViewCreateInfo({}, images[i],
                                                        vk::ImageViewType::e2D,
                                                        format.format,
                                                        vk::ComponentMapping(
                                                                vk::ComponentSwizzle::eIdentity,
                                                                vk::ComponentSwizzle::eIdentity,
                                                                vk::ComponentSwizzle::eIdentity,
                                                                vk::ComponentSwizzle::eIdentity),
                                                        vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor,
                                                                                  0, 1, 0, 1));
            imageViews[i] = context->getDevice()->getVkDevice().createImageView(imageViewCreateInfo);
        }
    }

    Swapchain::~Swapchain() {
        for (auto imageView: imageViews) {
            context->getDevice()->getVkDevice().destroyImageView(imageView);
        }
        context->getDevice()->getVkDevice().destroySwapchainKHR(swapchain);
    }

    void Swapchain::rebuild(uint32_t *preferredDimensions) {
        for (auto imageView: imageViews) {
            context->getDevice()->getVkDevice().destroyImageView(imageView);
        }
        imageViews.clear();

        SwapchainSupportDetails details{context->getDevice()->getPhysicalDevice(),
                                        context->getSurface()->getSurface()};
        extent = details.getOptimalExtent(preferredDimensions);

        vk::SwapchainCreateInfoKHR createInfo({}, context->getSurface()->getSurface(),
                                              imageCount,
                                              format.format,
                                              format.colorSpace,
                                              extent,
                                              1,
                                              vk::ImageUsageFlagBits::eColorAttachment);

        if (context->getDevice()->getGraphicsQueueFamily() != context->getDevice()->getPresentQueueFamily()) {
            uint32_t familyIndices[2] = {context->getDevice()->getGraphicsQueueFamily(),
                                         context->getDevice()->getPresentQueueFamily()};
            createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = familyIndices;
        } else {
            createInfo.imageSharingMode = vk::SharingMode::eExclusive;
        }

        createInfo.preTransform = surfaceCapabilities.currentTransform;
        createInfo.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        createInfo.presentMode = presentMode;
        createInfo.clipped = true;

        auto oldSwapchain = swapchain;
        createInfo.oldSwapchain = oldSwapchain;

        swapchain = context->getDevice()->getVkDevice().createSwapchainKHR(createInfo);

        images = context->getDevice()->getVkDevice().getSwapchainImagesKHR(swapchain);

        imageViews.resize(images.size());
        for (size_t i = 0; i < imageViews.size(); i++) {
            vk::ImageViewCreateInfo imageViewCreateInfo({}, images[i],
                                                        vk::ImageViewType::e2D,
                                                        format.format,
                                                        vk::ComponentMapping(
                                                                vk::ComponentSwizzle::eIdentity,
                                                                vk::ComponentSwizzle::eIdentity,
                                                                vk::ComponentSwizzle::eIdentity,
                                                                vk::ComponentSwizzle::eIdentity),
                                                        vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor,
                                                                                  0, 1, 0, 1));
            imageViews[i] = context->getDevice()->getVkDevice().createImageView(imageViewCreateInfo);
        }
        context->getDevice()->getVkDevice().destroySwapchainKHR(oldSwapchain);
    }

}


namespace vulkan {

    SwapchainSupportDetails::SwapchainSupportDetails(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface) {
        capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);
        formats = physicalDevice.getSurfaceFormatsKHR(surface);
        presentModes = physicalDevice.getSurfacePresentModesKHR(surface);
    }

    vk::SurfaceFormatKHR SwapchainSupportDetails::getOptimalSurfaceFormat() {
        for (auto format: formats) {
            if (format.format == vk::Format::eB8G8R8A8Unorm && format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
                return format;
            }
        }
        return formats[0];
    }

    vk::PresentModeKHR SwapchainSupportDetails::getOptimalPresentMode() {
        for (const auto &presentMode: presentModes) {
            if (presentMode == vk::PresentModeKHR::eFifo) {
                return presentMode;
            }
        }
        return vk::PresentModeKHR::eFifo;
    }

    vk::Extent2D SwapchainSupportDetails::getOptimalExtent(uint32_t *preferredDimensions) {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        }

        auto width = std::max(std::min(preferredDimensions[0], capabilities.maxImageExtent.width),
                              capabilities.minImageExtent.width);
        auto height = std::max(std::min(preferredDimensions[1], capabilities.maxImageExtent.height),
                               capabilities.minImageExtent.height);

        return vk::Extent2D{width, height};
    }

    uint32_t SwapchainSupportDetails::getOptimalImageCount() {
        uint32_t maxImageCount = capabilities.maxImageCount;
        uint32_t preferredImageCount = capabilities.minImageCount + 1;

        if (maxImageCount > 0 && preferredImageCount > maxImageCount) {
            preferredImageCount = maxImageCount;
        }
        return preferredImageCount;
    }

    vk::SurfaceCapabilitiesKHR SwapchainSupportDetails::getCapabilities() {
        return capabilities;
    }

    std::vector<vk::SurfaceFormatKHR> SwapchainSupportDetails::getSurfaceFormats() {
        return formats;
    }

    std::vector<vk::PresentModeKHR> SwapchainSupportDetails::getPresentModes() {
        return presentModes;
    }

}