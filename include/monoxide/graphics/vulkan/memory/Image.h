#pragma once

#include "monoxide/graphics/vulkan/core/Context.h"

namespace vulkan {
    class Image;
}

class vulkan::Image {
public:
    Image(const std::shared_ptr<Context>& context, vk::ImageCreateInfo parameters, VmaAllocationCreateInfo allocationInfo);

    ~Image();

    VmaAllocation allocation;

    vk::Image image;
private:
    std::shared_ptr<Context> context;

};