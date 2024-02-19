CPMFindPackage(
        NAME Freetype
        GITHUB_REPOSITORY freetype/freetype
        GIT_TAG 47574f7ea445c8bb751da0fa716424c9c29a6807
        OPTIONS
        "FT_DISABLE_ZLIB ON"
        "FT_DISABLE_BZIP2 ON"
        "FT_DISABLE_PNG ON"
        "FT_DISABLE_HARFBUZZ OFF"
        "FT_DISABLE_BROTLI ON"
)

if (Freetype_ADDED)
    add_library(Freetype::Freetype ALIAS freetype)
endif()