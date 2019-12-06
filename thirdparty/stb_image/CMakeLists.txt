cmake_minimum_required(VERSION 3.0)
project(stb_image)

add_library(stb_image INTERFACE)

target_include_directories(stb_image INTERFACE include/)
target_sources(stb_image INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include/stb_image.h)