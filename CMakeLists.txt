cmake_minimum_required(VERSION 3.28)
project(PPU C)

set(CMAKE_C_STANDARD 23)

include(${CMAKE_SOURCE_DIR}/CPM.cmake)
set(SDL_STATIC ON)

CPMAddPackage(
        NAME SDL
        GITHUB_REPOSITORY
        libsdl-org/SDL
        GIT_TAG SDL2)

add_executable(PPU
            Source/main.c


        Source/Graphics/Framebuffer.c
        Source/Graphics/Framebuffer.h
        Source/Graphics/ROP/ROP.c
        Source/Graphics/ROP/ROP.h
)

target_link_libraries(${PROJECT_NAME} SDL2::SDL2-static)