


add_executable(hello_triangle main.cpp)
target_link_libraries(
    hello_triangle PRIVATE 
    
    glbinding::glbinding 
    glbinding::glbinding-aux 

    glfw

    spdlog::spdlog
)

