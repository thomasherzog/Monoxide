#include "monoxide/renderer/ImGuiRenderer.h"

ImGuiRenderer::ImGuiRenderer(const std::shared_ptr<vulkan::Context>& context, GLFWwindow *window, vulkan::Swapchain &swapchain)
        : context(context) {
    vk::DescriptorPoolSize poolSizes[] = {
            {vk::DescriptorType::eSampler,              1000},
            {vk::DescriptorType::eCombinedImageSampler, 1000},
            {vk::DescriptorType::eSampledImage,         1000},
            {vk::DescriptorType::eStorageImage,         1000},
            {vk::DescriptorType::eUniformTexelBuffer,   1000},
            {vk::DescriptorType::eStorageTexelBuffer,   1000},
            {vk::DescriptorType::eUniformBuffer,        1000},
            {vk::DescriptorType::eStorageBuffer,        1000},
            {vk::DescriptorType::eUniformBufferDynamic, 1000},
            {vk::DescriptorType::eStorageBufferDynamic, 1000},
            {vk::DescriptorType::eInputAttachment,      1000}
    };

    vk::DescriptorPoolCreateInfo poolCreateInfo(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
                                                1000 * IM_ARRAYSIZE(poolSizes),
                                                (uint32_t) IM_ARRAYSIZE(poolSizes), poolSizes);

    descriptorPool = context->getDevice()->getVkDevice().createDescriptorPool(poolCreateInfo);

    imguiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(imguiContext);

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.IniFilename = nullptr;

    createCommandPools(swapchain.imageCount);
    createCommandBuffers(swapchain.imageCount);
    createRenderPass(swapchain);

    for (size_t i = 0; i < swapchain.imageCount; i++) {
        vk::FramebufferCreateInfo framebufferCreateInfo({}, renderPass, swapchain.imageViews[i],
                                                        swapchain.extent.width, swapchain.extent.height, 1);
        framebuffers.push_back(context->getDevice()->getVkDevice().createFramebuffer(framebufferCreateInfo));
    }

    ImGui_ImplGlfw_InitForVulkan(window, true);
    ImGui_ImplVulkan_InitInfo vulkanInitInfo{};
    vulkanInitInfo.Instance = context->getInstance()->getInstance();
    vulkanInitInfo.PhysicalDevice = context->getDevice()->getPhysicalDevice();
    vulkanInitInfo.Device = context->getDevice()->getVkDevice();
    vulkanInitInfo.QueueFamily = context->getDevice()->getGraphicsQueueFamily();
    vulkanInitInfo.Queue = context->getDevice()->getGraphicsQueue();
    vulkanInitInfo.DescriptorPool = descriptorPool;
    vulkanInitInfo.MinImageCount = swapchain.imageCount;
    vulkanInitInfo.ImageCount = swapchain.imageCount;
    vulkanInitInfo.CheckVkResultFn = [](VkResult result) {
        if (result != VK_SUCCESS) {
            throw std::runtime_error("Internal ImGui error");
        }
    };
    ImGui_ImplVulkan_Init(&vulkanInitInfo, renderPass);

    ImGui::GetIO().Fonts->AddFontDefault();
    context->executeTransient([](vk::CommandBuffer commandBuffer) {
        return ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
    });
    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

ImGuiRenderer::~ImGuiRenderer() {
    context->getDevice()->getVkDevice().waitIdle();

    ImGui::SetCurrentContext(imguiContext);
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(imguiContext);

    for (auto &framebuffer: framebuffers) {
        context->getDevice()->getVkDevice().destroyFramebuffer(framebuffer);
    }
    context->getDevice()->getVkDevice().destroyRenderPass(renderPass);
    for (auto &commandPool: commandPools) {
        context->getDevice()->getVkDevice().destroyCommandPool(commandPool);
    }
    context->getDevice()->getVkDevice().destroyDescriptorPool(descriptorPool);
}

void ImGuiRenderer::declareUserInterface(const std::function<void()> &lambda) {
    ImGui::SetCurrentContext(imguiContext);
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    lambda();

    ImGui::Render();
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

RecordedCommandBuffer ImGuiRenderer::recordCommandBuffer(vulkan::Swapchain &swapchain, int imageIndex) {
    context->getDevice()->getVkDevice().resetCommandPool(commandPools[imageIndex]);

    auto commandBuffer = commandBuffers[imageIndex];
    commandBuffer.begin(vk::CommandBufferBeginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit));
    vk::ClearValue clearValue{std::array<float, 4>{0.2f, 1.0f, 0.2f, 0.1f}};
    commandBuffer.beginRenderPass(vk::RenderPassBeginInfo(
            renderPass,
            framebuffers[imageIndex],
            vk::Rect2D({}, swapchain.extent),
            clearValue
    ), vk::SubpassContents::eInline);

    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);

    commandBuffer.endRenderPass();
    commandBuffer.end();

    return {context->getDevice()->getGraphicsQueue(), commandBuffer};
}

void ImGuiRenderer::createCommandPools(int imagesInFlight) {
    for (int i = 0; i < imagesInFlight; ++i) {
        commandPools.emplace_back(
                context->getDevice()->getVkDevice().createCommandPool(
                        vk::CommandPoolCreateInfo({}, context->getDevice()->getGraphicsQueueFamily())));

    }
}

void ImGuiRenderer::createCommandBuffers(int imagesInFlight) {
    commandBuffers.resize(imagesInFlight);
    for (int i = 0; i < imagesInFlight; ++i) {
        commandBuffers[i] = context->getDevice()->getVkDevice().allocateCommandBuffers(
                vk::CommandBufferAllocateInfo(commandPools[i], vk::CommandBufferLevel::ePrimary, 1))[0];
    }
}

void ImGuiRenderer::createRenderPass(vulkan::Swapchain &swapchain) {
    vk::AttachmentDescription attachmentDescription({}, swapchain.format.format,
                                                    vk::SampleCountFlagBits::e1,
                                                    vk::AttachmentLoadOp::eDontCare,
                                                    vk::AttachmentStoreOp::eStore,
                                                    vk::AttachmentLoadOp::eDontCare,
                                                    vk::AttachmentStoreOp::eDontCare,
                                                    vk::ImageLayout::eUndefined,
                                                    vk::ImageLayout::ePresentSrcKHR);

    vk::AttachmentReference attachmentReference(0, vk::ImageLayout::eColorAttachmentOptimal);

    vk::SubpassDescription subpassDescription({}, vk::PipelineBindPoint::eGraphics, {}, attachmentReference);

    vk::SubpassDependency dependency(VK_SUBPASS_EXTERNAL, {},
                                     vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                     vk::PipelineStageFlagBits::eColorAttachmentOutput,
                                     {}, vk::AccessFlagBits::eColorAttachmentWrite, {});

    vk::RenderPassCreateInfo renderPassCreateInfo({}, attachmentDescription, subpassDescription, dependency);

    renderPass = context->getDevice()->getVkDevice().createRenderPass(renderPassCreateInfo);
}

void ImGuiRenderer::onSwapchainRebuild(vulkan::Swapchain &swapchain) {
    context->getDevice()->getVkDevice().destroyRenderPass(renderPass);
    createRenderPass(swapchain);

    for (auto &framebuffer: framebuffers) {
        context->getDevice()->getVkDevice().destroyFramebuffer(framebuffer);
    }

    framebuffers.clear();
    for (size_t i = 0; i < swapchain.imageCount; i++) {
        vk::FramebufferCreateInfo framebufferCreateInfo({}, renderPass, swapchain.imageViews[i],
                                                        swapchain.extent.width, swapchain.extent.height, 1);
        framebuffers.push_back(context->getDevice()->getVkDevice().createFramebuffer(framebufferCreateInfo));
    }
}
