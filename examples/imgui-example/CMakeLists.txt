cmake_minimum_required(VERSION 3.22)
project(Monoxide-ImGui-Example)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_executable(${PROJECT_NAME}
        src/main.cpp
        src/window/ImGuiExampleWindow.cpp
        )
target_link_libraries(${PROJECT_NAME} Monoxide)