#include "monoxide/graphics/vulkan/render/RecordedCommandBuffer.h"

RecordedCommandBuffer::RecordedCommandBuffer(vk::Queue queue, vk::CommandBuffer commandBuffer)
        : queue(queue), commandBuffer(commandBuffer) {}

void RecordedCommandBuffer::submit(std::vector<vk::Semaphore> waitSemaphores,
                                   std::vector<vk::PipelineStageFlags> waitDstStageMask,
                                   std::vector<vk::Semaphore> signalSemaphore, vk::Fence fence) {
    vk::SubmitInfo submitInfo(
            waitSemaphores,
            waitDstStageMask,
            commandBuffer,
            signalSemaphore
    );
    queue.submit(submitInfo, fence);
}