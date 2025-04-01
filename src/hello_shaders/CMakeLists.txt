


add_executable(hello_shaders main.cpp)
target_link_libraries(
    hello_shaders PRIVATE 
    
    glbinding::glbinding 
    glbinding::glbinding-aux 

    glfw

    spdlog::spdlog

    omgl

    glm::glm
)

