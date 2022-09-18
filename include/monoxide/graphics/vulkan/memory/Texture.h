#ifndef MONOXIDE_TEXTURE_H
#define MONOXIDE_TEXTURE_H

#include "monoxide/graphics/vulkan/memory/Image.h"
#include "monoxide/graphics/vulkan/memory/Buffer.h"

namespace vulkan {
    class Texture;
}

class vulkan::Texture {
public:

    Texture(const std::shared_ptr<Context> &context, std::unique_ptr<Image> image, vk::SamplerCreateInfo samplerInfo,
            vk::ImageViewCreateInfo imageViewInfo);

    Texture(const std::shared_ptr<Context> &context, int width, int height, void* pixels);

    ~Texture();

    vk::ImageView imageView;

    vk::Sampler sampler;
private:
    std::shared_ptr<Context> context;

    std::unique_ptr<Image> image;

};


#endif //MONOXIDE_TEXTURE_H
