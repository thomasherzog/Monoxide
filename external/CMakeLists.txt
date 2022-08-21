find_package(Vulkan REQUIRED)

add_subdirectory(imgui)
add_subdirectory(imgui_impl)

add_subdirectory(cpm)


target_link_libraries(${PROJECT_NAME} PUBLIC
        Vulkan::Vulkan
        glfw
        VulkanMemoryAllocator
        freetype
        imgui
        imgui_impl

        )