CPMFindPackage(
        NAME harfbuzz
        GITHUB_REPOSITORY harfbuzz/harfbuzz
        GIT_TAG 41f85d0179dd60ee49e3064213bd4bbf5edaec96
)

if (harfbuzz_ADDED)
    add_library(harfbuzz::harfbuzz ALIAS harfbuzz)
endif()