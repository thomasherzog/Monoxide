#include "monoxide/graphics/vulkan/memory/Buffer.h"

namespace vulkan {

    Buffer::Buffer(const std::shared_ptr<Context> &context, vk::BufferCreateInfo bufferInfo,
                   VmaAllocationCreateInfo allocationInfo) : context(context) {
        vmaCreateBuffer(
                context->getAllocator(),
                reinterpret_cast<const VkBufferCreateInfo *>(&bufferInfo),
                &allocationInfo,
                reinterpret_cast<VkBuffer *>(&buffer),
                &allocation,
                nullptr
        );
    }

    Buffer::~Buffer() {
        vmaDestroyBuffer(context->getAllocator(), buffer, allocation);
    }
}