


add_executable(square main.cpp)
target_link_libraries(
    square PRIVATE 
    
    glbinding::glbinding 
    glbinding::glbinding-aux 

    glfw

    spdlog::spdlog

    omgl
)

