#include "monoxide/graphics/vulkan/memory/Texture.h"


namespace vulkan {

    Texture::Texture(const std::shared_ptr<Context> &context, std::unique_ptr<Image> image,
                     vk::SamplerCreateInfo samplerInfo,
                     vk::ImageViewCreateInfo imageViewInfo) : context(context), image(std::move(image)) {
        sampler = context->getDevice()->getVkDevice().createSampler(samplerInfo);
        imageView = context->getDevice()->getVkDevice().createImageView(imageViewInfo);
    }

    Texture::~Texture() {
        context->getDevice()->getVkDevice().destroy(imageView);
        context->getDevice()->getVkDevice().destroy(sampler);
    }

    Texture::Texture(const std::shared_ptr<Context> &context, int width, int height, void *pixels)
            : context(context) {
        vk::DeviceSize imageSize = width * height * 4;
        vk::Extent3D extent{static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1};
        vk::Format format = vk::Format::eR8G8B8A8Srgb;

        vk::BufferCreateInfo stagingBufferInfo{{}, imageSize, vk::BufferUsageFlagBits::eTransferSrc};
        VmaAllocationCreateInfo stagingAllocationInfo{
                VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT
                | VMA_ALLOCATION_CREATE_MAPPED_BIT,
                VMA_MEMORY_USAGE_AUTO
        };

        Buffer stagingBuffer{context, stagingBufferInfo, stagingAllocationInfo};

        void *data;
        vmaMapMemory(context->getAllocator(), stagingBuffer.allocation, &data);
        memcpy(data, pixels, imageSize);
        vmaUnmapMemory(context->getAllocator(), stagingBuffer.allocation);

        vk::ImageCreateInfo imageInfo{
                {},
                vk::ImageType::e2D,
                format,
                extent,
                1, 1, vk::SampleCountFlagBits::e1, vk::ImageTiling::eOptimal,
                vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled
        };
        VmaAllocationCreateInfo allocationInfo{
                VMA_ALLOCATION_CREATE_DEDICATED_MEMORY_BIT,
                VMA_MEMORY_USAGE_AUTO
        };
        image = std::make_unique<Image>(context, imageInfo, allocationInfo);

        context->executeTransient([this, &extent, &stagingBuffer](vk::CommandBuffer commandBuffer) {

            vk::ImageMemoryBarrier barrier{
                    vk::AccessFlagBits::eNone,
                    vk::AccessFlagBits::eTransferWrite,
                    vk::ImageLayout::eUndefined,
                    vk::ImageLayout::eTransferDstOptimal,
                    {},
                    {},
                    image->image,
                    vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1)
            };
            commandBuffer.pipelineBarrier(
                    vk::PipelineStageFlagBits::eTopOfPipe,
                    vk::PipelineStageFlagBits::eTransfer,
                    {},
                    nullptr,
                    nullptr,
                    barrier
            );

            vk::BufferImageCopy copy{
                    0, 0, 0,
                    vk::ImageSubresourceLayers(vk::ImageAspectFlagBits::eColor, 0, 0, 1),
                    {}, extent
            };
            commandBuffer.copyBufferToImage(
                    stagingBuffer.buffer,
                    image->image,
                    vk::ImageLayout::eTransferDstOptimal,
                    copy
            );

            barrier.oldLayout = vk::ImageLayout::eTransferDstOptimal;
            barrier.newLayout = vk::ImageLayout::eShaderReadOnlyOptimal;
            barrier.srcAccessMask = vk::AccessFlagBits::eTransferWrite;
            barrier.dstAccessMask = vk::AccessFlagBits::eShaderRead;
            commandBuffer.pipelineBarrier(
                    vk::PipelineStageFlagBits::eTransfer,
                    vk::PipelineStageFlagBits::eFragmentShader,
                    {},
                    nullptr,
                    nullptr,
                    barrier
            );

            return 1;
        });

        vk::SamplerCreateInfo samplerInfo{
                {},
                vk::Filter::eLinear,
                vk::Filter::eLinear,
        };

        vk::ImageViewCreateInfo imageViewInfo{
                {},
                image->image,
                vk::ImageViewType::e2D,
                format,
                vk::ComponentMapping{
                        vk::ComponentSwizzle::eR,
                        vk::ComponentSwizzle::eG,
                        vk::ComponentSwizzle::eB,
                        vk::ComponentSwizzle::eA
                },
                vk::ImageSubresourceRange{vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}
        };
        sampler = context->getDevice()->getVkDevice().createSampler(samplerInfo);
        imageView = context->getDevice()->getVkDevice().createImageView(imageViewInfo);
    }


}