add_library(
    omgl STATIC

    src/omgl/shaders.cpp
    include/omgl/shaders.hpp

    include/omgl/io.hpp

    src/omgl/glfw.cpp
    include/omgl/glfw.hpp
)

target_link_libraries(
    omgl PRIVATE

    glbinding::glbinding 
    glbinding::glbinding-aux 

    spdlog::spdlog
)

target_include_directories(
    omgl PUBLIC

    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

