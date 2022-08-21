#ifndef MONOXIDE_RECORDEDCOMMANDBUFFER_H
#define MONOXIDE_RECORDEDCOMMANDBUFFER_H

#include "monoxide/graphics/vulkan/core/Context.h"

class RecordedCommandBuffer {
public:
    RecordedCommandBuffer(vk::Queue queue, vk::CommandBuffer commandBuffer);

    void submit(std::vector<vk::Semaphore> waitSemaphores,
                std::vector<vk::PipelineStageFlags> waitDstStageMask,
                std::vector<vk::Semaphore> signalSemaphore,
                vk::Fence fence);

private:
    vk::Queue queue;

    vk::CommandBuffer commandBuffer;

};


#endif //MONOXIDE_RECORDEDCOMMANDBUFFER_H
