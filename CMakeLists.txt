cmake_minimum_required(VERSION 3.9)
project(SDL_FixedFPS_Variable_Render)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake_modules")

set(CMAKE_CXX_STANDARD 11)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR})

set(MAIN_SOURCE_FILES src/main.cpp src/Game.cpp src/Game.h)
add_executable(SDL_FixedFPS_Variable_Render ${MAIN_SOURCE_FILES})

target_link_libraries(SDL_FixedFPS_Variable_Render ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES})