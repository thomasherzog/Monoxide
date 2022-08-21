list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

list(APPEND cpm_packages
        glfw
        VulkanMemoryAllocator
        freetype
        )

include(CPM)

foreach (package IN LISTS cpm_packages)
    include(${CMAKE_CURRENT_SOURCE_DIR}/packages/${package}.cmake)
endforeach ()


