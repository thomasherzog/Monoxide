#include "monoxide/graphics/vulkan/core/InFlightFrames.h"

namespace vulkan {

    InFlightFrames::InFlightFrames(const std::shared_ptr<Context>& context, int maxFramesInFlight) : context(context) {
        imageAvailableSemaphores.resize(maxFramesInFlight);
        renderFinishedSemaphores.resize(maxFramesInFlight);
        fences.resize(maxFramesInFlight);

        for (int i = 0; i < maxFramesInFlight; ++i) {
            imageAvailableSemaphores[i] = context->getDevice()->getVkDevice().createSemaphore(
                    vk::SemaphoreCreateInfo());

            renderFinishedSemaphores[i] = context->getDevice()->getVkDevice().createSemaphore(
                    vk::SemaphoreCreateInfo());

            fences[i] = context->getDevice()->getVkDevice().createFence(
                    vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));
        }
    }

    InFlightFrames::~InFlightFrames() {
        for (int i = 0; i < fences.size(); ++i) {
            context->getDevice()->getVkDevice().destroySemaphore(renderFinishedSemaphores[i]);
            context->getDevice()->getVkDevice().destroySemaphore(imageAvailableSemaphores[i]);
            context->getDevice()->getVkDevice().destroyFence(fences[i]);
        }
    }

    SyncObject InFlightFrames::getNextSyncObject() {
        vk::Semaphore imageAvailableSemaphore = imageAvailableSemaphores[currentFrame];
        vk::Semaphore renderFinishedSemaphore = renderFinishedSemaphores[currentFrame];
        vk::Fence fence = fences[currentFrame];

        currentFrame = (currentFrame + 1) % fences.size();

        return SyncObject{
                imageAvailableSemaphore,
                renderFinishedSemaphore,
                fence
        };
    }

}