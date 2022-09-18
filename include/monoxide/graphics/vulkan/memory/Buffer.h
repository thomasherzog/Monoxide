#ifndef MONOXIDE_BUFFER_H
#define MONOXIDE_BUFFER_H

#include "monoxide/graphics/vulkan/core/Context.h"

namespace vulkan {
    class Buffer;
}

class vulkan::Buffer {

public:
    Buffer(const std::shared_ptr<Context> &context, vk::BufferCreateInfo bufferInfo,
           VmaAllocationCreateInfo allocationInfo);

    ~Buffer();

    VmaAllocation allocation;

    vk::Buffer buffer;
private:
    std::shared_ptr<Context> context;

};


#endif //MONOXIDE_BUFFER_H
