CPMFindPackage(
        NAME harfbuzz
        GITHUB_REPOSITORY harfbuzz/harfbuzz
        GIT_TAG c546aa6c5ae78605fd048fd628bd1f25b2e3e50f
)

if (harfbuzz_ADDED)
    add_library(harfbuzz::harfbuzz ALIAS harfbuzz)
endif()