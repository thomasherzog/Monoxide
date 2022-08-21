CPMFindPackage(
        NAME VulkanMemoryAllocator
        GITHUB_REPOSITORY GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator
        GIT_TAG beb26af01cda4b68091d700826c77893f2fdbfd2
        DOWNLOAD_ONLY ON
)

if (VulkanMemoryAllocator_ADDED)
    add_library(VulkanMemoryAllocator INTERFACE)
    target_include_directories(VulkanMemoryAllocator INTERFACE ${VulkanMemoryAllocator_SOURCE_DIR}/include)
endif ()