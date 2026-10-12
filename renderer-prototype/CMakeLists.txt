cmake_minimum_required(VERSION 3.16)
project(sodium_linux_demo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(sodium-linux-demo src/main.cpp)
target_link_libraries(sodium-linux-demo PRIVATE SDL2::SDL2 OpenGL::GL)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(sodium-linux-demo PRIVATE -Wall -Wextra -Wpedantic)
endif()
