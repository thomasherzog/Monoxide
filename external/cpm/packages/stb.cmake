CPMAddPackage(
        NAME stb
        GITHUB_REPOSITORY nothings/stb
        GIT_TAG 8b5f1f37b5b75829fc72d38e7b5d4bcbf8a26d55
        DOWNLOAD_ONLY ON
)

if (stb_ADDED)
    add_library(stb INTERFACE)
    target_include_directories(stb INTERFACE ${stb_SOURCE_DIR})
endif ()