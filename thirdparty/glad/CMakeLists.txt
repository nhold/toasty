cmake_minimum_required(VERSION 3.0)
project(Glad)

add_library(glad include/glad/glad.h src/glad.c include/glad/glad_wgl.h src/glad_wgl.c)
target_include_directories(glad PUBLIC include/)