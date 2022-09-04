cmake_minimum_required(VERSION 3.22)
project(Monoxide)

set(CMAKE_CXX_STANDARD 20)

# =============
# CMake options
# =============

option(MONOXIDE_INCLUDE_EXAMPLES "Include examples" ON)
option(MONOXIDE_INCLUDE_EXTERNAL "Include external dependencies" ON)

# ==================
# Library definition
# ==================

include_directories(include)
add_library(${PROJECT_NAME}
        src/monoxide/windowing/WindowManager.cpp
        src/monoxide/windowing/BaseWindow.cpp
        src/monoxide/windowing/VulkanWindow.cpp
        src/monoxide/windowing/ImGuiWindow.cpp

        src/monoxide/graphics/vulkan/core/Context.cpp
        src/monoxide/graphics/vulkan/core/SharedContext.cpp
        src/monoxide/graphics/vulkan/core/Instance.cpp
        src/monoxide/graphics/vulkan/core/PhysicalDevice.cpp
        src/monoxide/graphics/vulkan/core/Device.cpp
        src/monoxide/graphics/vulkan/core/Surface.cpp
        src/monoxide/graphics/vulkan/core/InFlightFrames.cpp
        src/monoxide/graphics/vulkan/core/Swapchain.cpp

        src/monoxide/graphics/vulkan/debug/DebugMessenger.cpp

        src/monoxide/graphics/vulkan/render/RecordedCommandBuffer.cpp

        src/monoxide/graphics/vulkan/memory/Image.cpp

        src/monoxide/renderer/ImGuiRenderer.cpp
        )


# ======
# Extras
# ======

if (MONOXIDE_INCLUDE_EXAMPLES)
    add_subdirectory(examples)
endif ()

if (MONOXIDE_INCLUDE_EXTERNAL)
    add_subdirectory(external)
endif ()

target_compile_definitions(${PROJECT_NAME} PUBLIC VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1)
