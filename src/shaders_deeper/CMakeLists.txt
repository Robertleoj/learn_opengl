add_executable(shaders_deeper main.cpp)

target_link_libraries(
    shaders_deeper PRIVATE 
    
    glbinding::glbinding 
    glbinding::glbinding-aux 

    glfw

    spdlog::spdlog

    omgl
)

