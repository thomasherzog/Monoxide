CPMFindPackage(
        NAME Freetype
        GITHUB_REPOSITORY freetype/freetype
        GIT_TAG 7cd3f19f21cc9d600e3b765ef2058474d20233e2
        OPTIONS
        "FT_DISABLE_ZLIB ON"
        "FT_DISABLE_BZIP2 ON"
        "FT_DISABLE_PNG ON"
        "FT_DISABLE_HARFBUZZ ON"
        "FT_DISABLE_BROTLI ON"
)

if (Freetype_ADDED)
    add_library(Freetype::Freetype ALIAS freetype)
endif()