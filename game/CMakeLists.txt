SET(includefiles
"include/Window.hpp"
"include/Program.hpp"
"include/Matrix4.hpp")

SET(sourcefiles
"source/main.cpp"
"include/Vector2.hpp"
"include/Vector2.inl"
"include/String.hpp"
"include/String.inl"
"source/Window.cpp"
"source/Matrix4.cpp"
"source/Program.cpp"
)

find_package(OpenGL REQUIRED)

add_executable(game
        ${includefiles}
        ${sourcefiles}
)

target_link_libraries(game ${OPENGL_gl_LIBRARY} glad stb_image)

add_definitions(-DUNICODE -D_UNICODE)

set_property(TARGET game PROPERTY CXX_STANDARD 20)
set_property(TARGET game PROPERTY CXX_STANDARD_REQUIRED ON)

include_directories("include/")

set (assetDir "${CMAKE_SOURCE_DIR}/assets")
set (assetDestination "${CMAKE_CURRENT_BINARY_DIR}")

file(COPY "${assetDir}" DESTINATION "${assetDestination}")