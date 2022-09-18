#include "monoxide/graphics/vulkan/memory/Image.h"

namespace vulkan {

    Image::Image(const std::shared_ptr<Context>& context, vk::ImageCreateInfo parameters, VmaAllocationCreateInfo allocationInfo)
            : context(context) {
        vmaCreateImage(context->getAllocator(),
                       reinterpret_cast<const VkImageCreateInfo *>(&parameters),
                       &allocationInfo,
                       reinterpret_cast<VkImage *>(&image),
                       &allocation,
                       nullptr);
    }

    Image::~Image() {
        vmaDestroyImage(context->getAllocator(), image, allocation);
    }


}