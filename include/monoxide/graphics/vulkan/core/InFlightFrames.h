#ifndef MONOXIDE_INFLIGHTFRAMES_H
#define MONOXIDE_INFLIGHTFRAMES_H

#include "monoxide/graphics/vulkan/core/Context.h"

namespace vulkan {
    class InFlightFrames;

    struct SyncObject;
}

class vulkan::InFlightFrames {
public:
    InFlightFrames(const std::shared_ptr<Context>& context, int maxFramesInFlight);

    ~InFlightFrames();

    SyncObject getNextSyncObject();

private:
    std::shared_ptr<Context> context;

    uint32_t currentFrame = 0;

    std::vector<vk::Semaphore> imageAvailableSemaphores;

    std::vector<vk::Semaphore> renderFinishedSemaphores;

    std::vector<vk::Fence> fences;

};

struct vulkan::SyncObject {
    vk::Semaphore imageAvailableSemaphore;

    vk::Semaphore renderFinishedSemaphore;

    vk::Fence fence;
};


#endif //MONOXIDE_INFLIGHTFRAMES_H
