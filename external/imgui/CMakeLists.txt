add_library(imgui
        src/imgui.cpp
        src/imgui_draw.cpp
        src/imgui_widgets.cpp
        src/imgui_tables.cpp
        src/imgui_demo.cpp
        src/misc/freetype/imgui_freetype.cpp)

target_include_directories(imgui PUBLIC include)

list(APPEND libraries glfw)

if (WIN32)
    list(APPEND libraries imm32.lib)
endif ()

list(APPEND libraries freetype)


target_link_libraries(imgui PUBLIC ${libraries})