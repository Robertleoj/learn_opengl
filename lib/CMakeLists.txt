add_library(
    omgl SHARED

    src/omgl/shaders.cpp
    include/omgl/shaders.hpp

    include/omgl/io.hpp
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

